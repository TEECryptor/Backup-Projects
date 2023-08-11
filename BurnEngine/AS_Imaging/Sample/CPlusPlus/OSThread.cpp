#include "OSThread.h"



OSThreadReturn AS_API OSThread::EntryPoint(void*  data)
{
 OSThread* pThread = (OSThread*)data;
 pThread->m_bActive = true;
 pThread->ThreadEntry(); //initialize
 pThread->Run();
 pThread->ThreadExit(); //finalize

 pThread->m_bActive = false;
 return 0;
}

#if defined (_WIN32)  // using Win32 API

bool OSThread::CreateNewThread()
{
 m_hThread = CreateThread(NULL,
                          0,
                          (LPTHREAD_START_ROUTINE)EntryPoint,
                          (LPVOID) this,
                          CREATE_SUSPENDED,
                          (LPDWORD) &m_lpId);

 if(m_hThread == NULL)
  return false;

 return true;
}
void OSThread::Sleep(UInt32 msecs)
{
	::Sleep(msecs);
}

bool OSThread::Suspend()
{
 m_bActive = false;
 return(-1 != SuspendThread(m_hThread));//win32 API
}

bool OSThread::Kill()
{
 m_bActive = false;
 return (-1 != CloseHandle(m_hThread)); //win32 API
}

bool OSThread::Resume()
{

 return(-1 != ResumeThread(m_hThread)); //win32 API
}

bool OSThread::Wait()
{
 return (WAIT_OBJECT_0
         == WaitForSingleObject(m_hThread, INFINITE));
 //win32 API
}

OSMutex::OSMutex(const std::string& name)
{
	Handle = CreateMutexA(0, 0, name.c_str());
}
OSMutex::~OSMutex()
{
	CloseHandle(Handle);
}
bool OSMutex::Enter()
{

	return (-1 == WaitForSingleObject(Handle, INFINITE));
}
bool OSMutex::Exit()
{
	return (-1 == ReleaseMutex(Handle));

}

#else  // using Posix threads/mutexes

bool OSThread::CreateNewThread()
{
    pthread_mutex_init(&mMutex, 0);
    m_bActive = false;

    return true;
}
void OSThread::Sleep(UInt32 msecs)
{
    if (msecs < 60000)
        usleep(msecs*1000);
    else
        sleep(msecs/1000);
}

bool OSThread::Suspend()
{
    pthread_mutex_lock(&mMutex);
    m_bActive = false;
    pthread_mutex_unlock(&mMutex);
}

bool OSThread::Kill()
{
    m_bActive = false;
    pthread_mutex_unlock(&mMutex);
 
    pthread_mutex_destroy(&mMutex);

}

bool OSThread::Resume()
{
    pthread_mutex_lock(&mMutex);
    SInt32 res = pthread_create(&m_thread, 0, EntryPoint, (void *)this);
    pthread_mutex_unlock(&mMutex);
}

bool OSThread::Wait()
{

}

OSMutex::OSMutex(const std::string& name)
{
   m_bInitialized = false;
   pthread_mutexattr_t attr;

   pthread_mutexattr_init(&attr);
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

   if (pthread_mutex_init(&Handle, &attr) == 0)
      m_bInitialized = true;
   pthread_mutexattr_destroy(&attr);
}
OSMutex::~OSMutex()
{
   if (m_bInitialized)
      pthread_mutex_destroy(&Handle);
   m_bInitialized = false;
}
bool OSMutex::Enter()
{
  if (!m_bInitialized) return false;

  if (pthread_mutex_lock(&Handle) == 0)
     return true;

  return false;
}
bool OSMutex::Exit()
{
  if (!m_bInitialized) return false;
  
  if (pthread_mutex_unlock(&Handle) == 0)
     return true;
  return false;

}

#endif


