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
};


class imLocker {
public:
    imLocker(imMutex* mutex) : m_mutex(mutex->m_mutex);
    {
        SDL_LockMutex(m_mutex);
    }

    ~imLocker()
    {
        SDL_UnlockMutex(m_mutex);
    }

private:
    SDL_mutex* m_mutex;
};

#endif
