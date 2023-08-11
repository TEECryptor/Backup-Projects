
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

   WaitForSingleObject(info.hThread, INFINITE); //等待子线程安全退出
   CloseHandle(info.hThread);//关闭线程句柄

   return TRUE;
}

//---------------------------------------------------------------------
DWORD WINAPI AsyncWinINet::AsyncThread(LPVOID lpParameter)
{
   thread_info* p = (thread_info*)lpParameter;

   //a. 使用标记 INTERNET_FLAG_ASYNC 初始化 InternetOpen
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
   //WaitForSingleObject(p->hEvent[0], INFINITE);//等待回调函数设置成功事件
   InternetSetStatusCallback(p->hInternet, AsyncWinINet::AsyncInternetCallback);

   String sheadrs;
   for(it = iheadrs.begin(); it != iheadrs.end(); ++it)
   {
    sheadrs += it->first + ":" + it->second;
    if(it->second.find(StringUtil::enter) == String::npos) { sheadrs += StringUtil::enter; }
   }
   sheadrs += StringUtil::enter;
  
   DWORD start_time = timeGetTime();
   ResetEvent(p->hEvent[0]); //重置句柄被创建事件
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

    //读取返回文件头
    DWORD dwLength = 0;
    LPVOID lpOutBuffer = NULL;
    while(true) //读取response_headrs数据
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

    //e. 使用 HttpQueryInfo 分析头信息 HttpQueryInfo 使用非阻塞方式，所以不用等待
    DWORD dwStatusSize = sizeof(p->dwStatusCode);
    if (FALSE == HttpQueryInfo(p->hFile, //获取返回状态码
     HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
     &p->dwStatusCode, &dwStatusSize, NULL)) { break; }
   
    //判断状态码是不是 200
    if (HTTP_STATUS_OK != p->dwStatusCode) break;
   
    StringMap msgMap(p->pfp_param.begin(), p->pfp_param.end());
    msgMap["url"] = p->url;

    //获取返回的Content-Length
    //DWORD dwLengthSize = sizeof(p->dwContentLength); 
    //if (FALSE == HttpQueryInfo(p->hFile,
    //HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
    //&p->dwContentLength, &dwLengthSize, NULL)) { p->dwContentLength = 0; }

    //f. 使用标记 IRF_ASYNC 读数据 InternetReadFileEx
    //为了向主线程报告进度，我们设置每次读数据最多 1024 字节

    char lpvBuffer[1024];
    p->dwContentLength = 0; //Content-Length: 202749
    while(true)
    {
     INTERNET_BUFFERS i_buf = {0};
     i_buf.dwStructSize = sizeof(INTERNET_BUFFERS);
     i_buf.lpvBuffer = lpvBuffer;
     i_buf.dwBufferLength = 1024;
       
     //重置读数据事件
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
      //在网络传输速度快，步长较小的情况下，InternetReadFileEx 经常会直接返回成功，
      //因此要判断是否发生了用户要求终止子线程事件。
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
    InternetCloseHandle(p->hFile);//关闭 m_hFile
    while (!WaitExitEvent(p)) //等待句柄被关闭事件或者要求子线程退出事件
    {
     ResetEvent(p->hEvent[0]);
    }
   }

   //设置子线程退出事件，通知回调线程退出
   SetEvent(p->hEvent[2]);
  
   //等待回调线程安全退出
   //WaitForSingleObject(p->hCallbackThread, INFINITE);
   //CloseHandle(p->hCallbackThread);
  
   //注销回调函数
   InternetSetStatusCallback(p->hInternet, NULL);
   InternetCloseHandle(p->hInternet);

   return TRUE;
}

//------------------------------------------------------------------------------------
DWORD WINAPI AsyncWinINet::AsyncCallbackThread(LPVOID lpParameter)
{
   thread_info *p = (thread_info*)lpParameter;
   InternetSetStatusCallback(p->hInternet, AsyncWinINet::AsyncInternetCallback);

   //通知子线程回调函数设置成功，子线程可以继续工作
   SetEvent(p->hEvent[0]);

   //等待用户终止事件或者子线程结束事件
   //子线程结束前需要设置子线程结束事件，并等待回调线程结束
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
  
   //在我们的应用中，我们只关心下面三个状态
   switch(dwInternetStatus)
   {
   //句柄被创建
   case INTERNET_STATUS_HANDLE_CREATED:
    p->hFile = (HINTERNET)(((LPINTERNET_ASYNC_RESULT)
     (lpvStatusInformation))->dwResult);
    break;
  
   //句柄被关闭
   case INTERNET_STATUS_HANDLE_CLOSING:
    SetEvent(p->hEvent[1]);
    break;

   //一个请求完成，比如一次句柄创建的请求，或者一次读数据的请求
   case INTERNET_STATUS_REQUEST_COMPLETE:
    if (ERROR_SUCCESS == ((LPINTERNET_ASYNC_RESULT)
     (lpvStatusInformation))->dwError)
    { 
     //设置句柄被创建事件或者读数据成功完成事件
     SetEvent(p->hEvent[0]);
    }
    else
    { 
     //如果发生错误，则设置子线程退出事件 这里也是一个陷阱，经常会忽视处理这个错误，
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
   case WAIT_OBJECT_0://句柄被创建事件或者读数据请求成功完成事件
   case WAIT_OBJECT_0+1://句柄被关闭事件
   case WAIT_OBJECT_0+2://用户要求终止子线程事件或者发生错误事件
    break;
   }
   return WAIT_OBJECT_0 != dwRet;
}