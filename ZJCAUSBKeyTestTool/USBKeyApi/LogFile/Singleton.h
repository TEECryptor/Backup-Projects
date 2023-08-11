#pragma once

#define DECLARE_AS_SINGLETON(ClassName) friend class Singleton<ClassName>
#define InstanceOf(classname) classname::Instance()

template<typename T> 
class Singleton
{
protected:
	Singleton()
	{
	}
	static volatile	LONG m_lLocker;

private:
	Singleton & operator=(Singleton & o){} 
	Singleton & operator()(Singleton & o){}

public:
	virtual ~Singleton()
	{
	}

	static T& Instance()
	{
		while (::InterlockedExchange(&m_lLocker, 1)	!= 0)
		{
			Sleep(10);
		}
		static T instance;
		::InterlockedExchange(&m_lLocker, 0);
		return instance;
	}
};

template <typename T>
volatile LONG Singleton<T>::m_lLocker	= 0;
