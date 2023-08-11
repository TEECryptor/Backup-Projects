
#include <wininet.h>
#include <mmsystem.h>

class AsyncWinINet
{
public:
   typedef void (*notify_fp)(const StringMap&);
  
   class thread_info
   {
   public:
    thread_info(const String& _url,     //请求下载的地址(in)
     const StringMap& _request_headrs,   //请求头request_headrs(in)
     const notify_fp& _pfp,      //下载进度通知回调函数指针
     const StringMap& _pfp_param,
     String& _response_headrs,     //返回头response_headrs(out)  
     const String& _saved_filename,    //下载内容保存文件名(in)
     String& _response_content,     //返回内容(out)
     size_t _read_content_size)     //控制保存在response_content中内容的长度(in)) : 
     : request_headrs(_request_headrs), pfp(_pfp),
     pfp_param(_pfp_param),      //pfp函数传回参数
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

    String url;         //请求下载的地址(in)
    const StringMap& request_headrs;   //请求头request_headrs(in)
    const notify_fp& pfp;      //下载进度通知回调函数指针
    const StringMap& pfp_param;     //pfp函数传回参数

    String& response_headrs;     //返回头response_headrs(out)  
    const String& saved_filename;    //下载内容保存文件名(in)
    String& response_content;     //返回内容(out)
    size_t read_content_size;     //控制保存在response_content中内容的长度(in)
   };

   /*******************************************************************************
   * 函数：download
   * 功能：下载，返回WinINet_ERR_CODE值
   *   说明：关于notify_fp 类型说明: 函数的参数为StringMap类型，传回的变量名与变量值
   * 2007-12
   *******************************************************************************/
   static DWORD download(const String& url, //请求下载的地址(in)
    const StringMap& request_headrs,   //请求头request_headrs(in)
    const notify_fp& pfp,      //下载进度通知回调函数指针
    const StringMap& pfp_param,     //pfp函数传回参数
    String& response_headrs,     //返回头response_headrs(out)  
    const String& saved_filename,    //下载内容保存文件名(in)
    String& response_content,     //返回内容(out)
    size_t read_content_size = 0);    //控制保存在response_content中内容的长度(in)

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