
#include <wininet.h>
#include <mmsystem.h>

class AsyncWinINet
{
public:
   typedef void (*notify_fp)(const StringMap&);
  
   class thread_info
   {
   public:
    thread_info(const String& _url,     //�������صĵ�ַ(in)
     const StringMap& _request_headrs,   //����ͷrequest_headrs(in)
     const notify_fp& _pfp,      //���ؽ���֪ͨ�ص�����ָ��
     const StringMap& _pfp_param,
     String& _response_headrs,     //����ͷresponse_headrs(out)  
     const String& _saved_filename,    //�������ݱ����ļ���(in)
     String& _response_content,     //��������(out)
     size_t _read_content_size)     //���Ʊ�����response_content�����ݵĳ���(in)) : 
     : request_headrs(_request_headrs), pfp(_pfp),
     pfp_param(_pfp_param),      //pfp�������ز���
     response_headrs(_response_headrs), saved_filename(_saved_filename),
     response_content(_response_content), read_content_size(_read_content_size) 
    {
     this->response_headrs.clear();
     this->response_content.clear();
     this->url = StringUtil::EncodeURIComponent(_url);
     for(int i = 0; i < 3; ++i)
     {
      this->hEvent[i] = CreateEvent(NULL,TRUE,FALSE,NULL);
     }
    }

    HANDLE hThread;
    DWORD dwThreadID;
    HANDLE hCallbackThread;
    DWORD dwCallbackThreadID;
    HANDLE hEvent[3];
    LPVOID hInternet;
    LPVOID hFile;
    DWORD dwStatusCode;
    DWORD dwContentLength;

    String url;         //�������صĵ�ַ(in)
    const StringMap& request_headrs;   //����ͷrequest_headrs(in)
    const notify_fp& pfp;      //���ؽ���֪ͨ�ص�����ָ��
    const StringMap& pfp_param;     //pfp�������ز���

    String& response_headrs;     //����ͷresponse_headrs(out)  
    const String& saved_filename;    //�������ݱ����ļ���(in)
    String& response_content;     //��������(out)
    size_t read_content_size;     //���Ʊ�����response_content�����ݵĳ���(in)
   };

   /*******************************************************************************
   * ������download
   * ���ܣ����أ�����WinINet_ERR_CODEֵ
   *   ˵��������notify_fp ����˵��: �����Ĳ���ΪStringMap���ͣ����صı����������ֵ
   * 2007-12
   *******************************************************************************/
   static DWORD download(const String& url, //�������صĵ�ַ(in)
    const StringMap& request_headrs,   //����ͷrequest_headrs(in)
    const notify_fp& pfp,      //���ؽ���֪ͨ�ص�����ָ��
    const StringMap& pfp_param,     //pfp�������ز���
    String& response_headrs,     //����ͷresponse_headrs(out)  
    const String& saved_filename,    //�������ݱ����ļ���(in)
    String& response_content,     //��������(out)
    size_t read_content_size = 0);    //���Ʊ�����response_content�����ݵĳ���(in)

protected:
   static BOOL WaitExitEvent(thread_info *p);
   static DWORD WINAPI AsyncThread(LPVOID lpParameter);
   static DWORD WINAPI AsyncCallbackThread(LPVOID lpParameter);
   static VOID CALLBACK AsyncInternetCallback(HINTERNET hInternet,
    DWORD dwContext,
    DWORD dwInternetStatus,
    LPVOID lpvStatusInformation,
    DWORD dwStatusInformationLength);

};