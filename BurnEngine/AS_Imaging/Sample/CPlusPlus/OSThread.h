#ifndef __OSTHREAD_H__
#define __OSTHREAD_H__

#if defined _WIN32
#include <windows.h>
typedef unsigned int OSThreadReturn;
#else
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
typedef void* OSThreadReturn;
#endif

#include <string>

#include "AS_StorageTypes.h"
#include "AS_StorageError.h"



class OSThread
{
public:
	OSThread(){};
	~OSThread(){};

   //Thread Management
   bool CreateNewThread();
   bool Wait(); //Wait for thread to end
   bool Suspend(); //Suspend the thread

   bool Resume(); //Resume a suspended thread
   bool Kill(); //Terminate a thread
   bool IsActive(){ return m_bActive;}; //Check for activity
   void SetActive(bool active){m_bActive = active;};

   //override these functions in the derived class
   virtual AS_StorageError ThreadEntry()=0;
   virtual AS_StorageError ThreadExit()=0;
   virtual AS_StorageError Run()=0;
public:
   static void Sleep(UInt32 msecs);

private:
static OSThreadReturn AS_API EntryPoint(void * data);

#if defined (_WIN32)
   HANDLE m_hThread; //Thread handle
#else  // Posix threads
   pthread_t m_thread;
   pthread_mutex_t mMutex;
#endif

   bool m_bActive; //activity indicator
   bool m_bSignaled; // signal indicator
   UInt32 m_lpId; //Thread ID



};

class OSMutex
{
public:
   OSMutex(const std::string& name);
   ~OSMutex();
   bool Enter();
   bool Exit();
protected:
#if !defined (_WIN32)
bool m_bInitialized;
#endif

#if defined(WIN32)
   HANDLE Handle;
#elif defined __unix__
   pthread_mutex_t Handle;
#endif
};


#endif  //__OSTHREAD_H__