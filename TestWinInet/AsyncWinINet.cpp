
#include "FagexAsyncWinINet.h"

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Wininet.lib")

DWORD AsyncWinINet::download(const Fagex::String &url, const Fagex::StringMap &request_headrs, 
   const Fagex::AsyncWinINet::notify_fp &pfp, const Fagex::StringMap &pfp_param, Fagex::String &response_headrs, 
   const Fagex::String &saved_filename, Fagex::String &response_content, size_t read_content_size)
{
   thread_info info(url, request_headrs, pfp,
    pfp_param, response_headrs, saved_filename,
    response_content, read_content_size);

   info.hThread = CreateThread(NULL, 
    0,
    AsyncWinINet::AsyncThread,
    &info,
    NULL,
    &info.dwThreadID);

   WaitForSingleObject(info.hThread, INFINITE); //�ȴ����̰߳�ȫ�˳�
   CloseHandle(info.hThread);//�ر��߳̾��

   return TRUE;
}

//---------------------------------------------------------------------
DWORD WINAPI AsyncWinINet::AsyncThread(LPVOID lpParameter)
{
   thread_info* p = (thread_info*)lpParameter;

   //a. ʹ�ñ�� INTERNET_FLAG_ASYNC ��ʼ�� InternetOpen
   String user_agent("Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1; TencentTraveler ; .NET CLR 2.0.50727)");
   StringMap iheadrs(p->request_headrs.begin(), p->request_headrs.end());
   StringMap::iterator it = iheadrs.find("User-Agent");
   if(it == iheadrs.end()) iheadrs["User-Agent"] = user_agent;
   else user_agent = it->second;

   p->hInternet = InternetOpen(user_agent.c_str(),
    INTERNET_OPEN_TYPE_PRECONFIG,
    NULL,
    NULL,
    INTERNET_FLAG_ASYNC);

   //ResetEvent(p->hEvent[0]);
   //p->hCallbackThread = CreateThread(NULL,
   // 0,
   // AsyncWinINet::AsyncCallbackThread,
   // p,
   // NULL,
   // &p->dwCallbackThreadID);
   //WaitForSingleObject(p->hEvent[0], INFINITE);//�ȴ��ص��������óɹ��¼�
   InternetSetStatusCallback(p->hInternet, AsyncWinINet::AsyncInternetCallback);

   String sheadrs;
   for(it = iheadrs.begin(); it != iheadrs.end(); ++it)
   {
    sheadrs += it->first + ":" + it->second;
    if(it->second.find(StringUtil::enter) == String::npos) { sheadrs += StringUtil::enter; }
   }
   sheadrs += StringUtil::enter;
  
   DWORD start_time = timeGetTime();
   ResetEvent(p->hEvent[0]); //���þ���������¼�
   p->hFile = InternetOpenUrl(p->hInternet, p->url.c_str(), sheadrs.c_str(), sheadrs.length(), 
    INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_RELOAD, (DWORD)p);

   FILE *fp = fopen(p->saved_filename.c_str(), "w+");
   while(true)
   {
    if (NULL == p->hFile)
    {
     DWORD dwError = ::GetLastError();
     if (ERROR_IO_PENDING == dwError || ERROR_SUCCESS == dwError)
     {
      if (WaitExitEvent(p)) { break; }
     }
     else break;
    }

    //��ȡ�����ļ�ͷ
    DWORD dwLength = 0;
    LPVOID lpOutBuffer = NULL;
    while(true) //��ȡresponse_headrs����
    {
     if(!HttpQueryInfo(p->hFile, HTTP_QUERY_RAW_HEADERS_CRLF,
        lpOutBuffer, &dwLength, NULL))
     {
      DWORD err_code = GetLastError();
      if (err_code == ERROR_HTTP_HEADER_NOT_FOUND) break; 
      else if(err_code == ERROR_INSUFFICIENT_BUFFER)
      {
       lpOutBuffer = new char[dwLength];
       continue; 
      }
      else break;
     }
     break;
    }
    if(lpOutBuffer != NULL)
    {
     p->response_headrs.append((char*)lpOutBuffer,dwLength);
     delete [] lpOutBuffer;
    }

    //e. ʹ�� HttpQueryInfo ����ͷ��Ϣ HttpQueryInfo ʹ�÷�������ʽ�����Բ��õȴ�
    DWORD dwStatusSize = sizeof(p->dwStatusCode);
    if (FALSE == HttpQueryInfo(p->hFile, //��ȡ����״̬��
     HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
     &p->dwStatusCode, &dwStatusSize, NULL)) { break; }
   
    //�ж�״̬���ǲ��� 200
    if (HTTP_STATUS_OK != p->dwStatusCode) break;
   
    StringMap msgMap(p->pfp_param.begin(), p->pfp_param.end());
    msgMap["url"] = p->url;

    //��ȡ���ص�Content-Length
    //DWORD dwLengthSize = sizeof(p->dwContentLength); 
    //if (FALSE == HttpQueryInfo(p->hFile,
    //HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
    //&p->dwContentLength, &dwLengthSize, NULL)) { p->dwContentLength = 0; }

    //f. ʹ�ñ�� IRF_ASYNC ������ InternetReadFileEx
    //Ϊ�������̱߳�����ȣ���������ÿ�ζ�������� 1024 �ֽ�

    char lpvBuffer[1024];
    p->dwContentLength = 0; //Content-Length: 202749
    while(true)
    {
     INTERNET_BUFFERS i_buf = {0};
     i_buf.dwStructSize = sizeof(INTERNET_BUFFERS);
     i_buf.lpvBuffer = lpvBuffer;
     i_buf.dwBufferLength = 1024;
       
     //���ö������¼�
     ResetEvent(p->hEvent[0]);
     if (FALSE == InternetReadFileEx(p->hFile, &i_buf, IRF_ASYNC, (DWORD)p))
     {
      if (ERROR_IO_PENDING == ::GetLastError())
      {
       if (WaitExitEvent(p)) break; 
      }
      else break; 
     }
     else
     {
      //�����紫���ٶȿ죬������С������£�InternetReadFileEx ������ֱ�ӷ��سɹ���
      //���Ҫ�ж��Ƿ������û�Ҫ����ֹ���߳��¼���
      if (WAIT_OBJECT_0 == WaitForSingleObject(p->hEvent[2], 0))
      {
       ResetEvent(p->hEvent[2]);
       break;
      }
     }

     if(i_buf.dwBufferLength == 0) 
     {
      DWORD time = timeGetTime() - start_time;
      if(time != 0)
      {
       Real speed = (Real)p->dwContentLength;
       speed /= ((Real)time)/1000.0f;
       speed /= 1024.0f;
       msgMap["speed"] = StringUtil::toString((DWORD)speed);
      }
      if(p->pfp) p->pfp(msgMap);
      break;
     }
     if(fp)
     {
      fwrite(i_buf.lpvBuffer, sizeof(char), i_buf.dwBufferLength, fp);
     }
     if(p->read_content_size > p->response_content.size())
     {
      p->response_content.append((char*)i_buf.lpvBuffer, i_buf.dwBufferLength);
     }
     p->dwContentLength += i_buf.dwBufferLength;
    }
    break;
   }
  
   if(fp)
   {
    fflush(fp); fclose(fp); fp = NULL;
   }

   if(p->hFile)
   {
    InternetCloseHandle(p->hFile);//�ر� m_hFile
    while (!WaitExitEvent(p)) //�ȴ�������ر��¼�����Ҫ�����߳��˳��¼�
    {
     ResetEvent(p->hEvent[0]);
    }
   }

   //�������߳��˳��¼���֪ͨ�ص��߳��˳�
   SetEvent(p->hEvent[2]);
  
   //�ȴ��ص��̰߳�ȫ�˳�
   //WaitForSingleObject(p->hCallbackThread, INFINITE);
   //CloseHandle(p->hCallbackThread);
  
   //ע���ص�����
   InternetSetStatusCallback(p->hInternet, NULL);
   InternetCloseHandle(p->hInternet);

   return TRUE;
}

//------------------------------------------------------------------------------------
DWORD WINAPI AsyncWinINet::AsyncCallbackThread(LPVOID lpParameter)
{
   thread_info *p = (thread_info*)lpParameter;
   InternetSetStatusCallback(p->hInternet, AsyncWinINet::AsyncInternetCallback);

   //֪ͨ���̻߳ص��������óɹ������߳̿��Լ�������
   SetEvent(p->hEvent[0]);

   //�ȴ��û���ֹ�¼��������߳̽����¼�
   //���߳̽���ǰ��Ҫ�������߳̽����¼������ȴ��ص��߳̽���
   WaitForSingleObject(p->hEvent[2], INFINITE);

   return 0;
}

//----------------------------------------------------------------------------
VOID CALLBACK AsyncWinINet::AsyncInternetCallback(HINTERNET hInternet,
    DWORD dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength)
{
   thread_info* p = (thread_info*)dwContext;
  
   //�����ǵ�Ӧ���У�����ֻ������������״̬
   switch(dwInternetStatus)
   {
   //���������
   case INTERNET_STATUS_HANDLE_CREATED:
    p->hFile = (HINTERNET)(((LPINTERNET_ASYNC_RESULT)
     (lpvStatusInformation))->dwResult);
    break;
  
   //������ر�
   case INTERNET_STATUS_HANDLE_CLOSING:
    SetEvent(p->hEvent[1]);
    break;

   //һ��������ɣ�����һ�ξ�����������󣬻���һ�ζ����ݵ�����
   case INTERNET_STATUS_REQUEST_COMPLETE:
    if (ERROR_SUCCESS == ((LPINTERNET_ASYNC_RESULT)
     (lpvStatusInformation))->dwError)
    { 
     //���þ���������¼����߶����ݳɹ�����¼�
     SetEvent(p->hEvent[0]);
    }
    else
    { 
     //��������������������߳��˳��¼� ����Ҳ��һ�����壬��������Ӵ����������
     SetEvent(p->hEvent[2]);
    }
    break;

   case INTERNET_STATUS_CONNECTION_CLOSED:
    SetEvent(p->hEvent[2]);
    break;

   }
}

//--------------------------------------------------------------------
BOOL AsyncWinINet::WaitExitEvent(thread_info *p)
{
   DWORD dwRet = WaitForMultipleObjects(3, p->hEvent, FALSE, INFINITE);
  
   switch (dwRet)
   {
   case WAIT_OBJECT_0://����������¼����߶���������ɹ�����¼�
   case WAIT_OBJECT_0+1://������ر��¼�
   case WAIT_OBJECT_0+2://�û�Ҫ����ֹ���߳��¼����߷��������¼�
    break;
   }
   return WAIT_OBJECT_0 != dwRet;
}