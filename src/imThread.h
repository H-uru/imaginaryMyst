#ifndef _IM_THREAD_H
#define _IM_THREAD_H

#include <SDL_thread.h>

class imMutex {
public:
    imMutex()
    {
        m_mutex = SDL_CreateMutex();
    }

    ~imMutex()
    {
        SDL_DestroyMutex(m_mutex);
    }

    void lock()
    {
        SDL_LockMutex(m_mutex);
    }

    void unlock()
    {
        SDL_UnlockMutex(m_mutex);
    }

private:
    SDL_mutex* m_mutex;
    friend class imLocker;
};


class imLocker {
public:
    imLocker(imMutex& mutex) : m_mutex(mutex)
    { m_mutex.lock(); }

    ~imLocker()
    { m_mutex.unlock(); }

private:
    imMutex& m_mutex;
};

#define LOCK_FUNCTION \
    static imMutex _mutex_lock; \
    imLocker _function_locker(_mutex_lock);

#endif
