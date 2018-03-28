/* This file is part of imaginaryMyst.
 *
 * imaginaryMyst is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * imaginaryMyst is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with imaginaryMyst.  If not, see <http://www.gnu.org/licenses/>.
 */

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
