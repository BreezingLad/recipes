/*************************************************************************
	> File Name: lockGuad.h
	> Author: liulianglu
	> Created Time: Fri 25 Aug 2017 11:38:40 PM EDT
 ************************************************************************/

#include<pthread.h>

class LockGuad
{
public:

	LockGuad(){	pthread_mutex_init(&m_mutex, NULL); }
	~LockGuad(){pthread_mutex_destroy(&m_mutex); }
	
	class CGuad
	{
	public:
		CGuad(LockGuad& lg):m_lock(lg) { m_lock.lock(); }
        ~CGuad() { m_lock.unlock(); }
    private:
        LockGuad& m_lock;
	};

private:
    pthread_mutex_t m_mutex;
    void lock() {pthread_mutex_lock(&m_mutex);}
    void unlock(){pthread_mutex_unlock(&m_mutex);}
    friend class LockGuad::CGuad;

};
