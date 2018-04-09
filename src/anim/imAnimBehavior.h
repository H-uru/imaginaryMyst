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

#ifndef _IM_ANIMBEHAVIOR_H
#define _IM_ANIMBEHAVIOR_H

#include <vector>
#include "vfs/imStream.h"
#include "imController.h"

class imAnimController {
public:
    enum Type {
        kTypeInvalid, kTypeScalar, kTypeColor, kTypeMatrix44
    };

    imAnimController(Type type) : m_type(type), m_controller(0)
    { }

    ~imAnimController()
    {
        if (m_controller != 0)
            delete m_controller;
    }

    void read(imStream* stream);

    Type type() const
    { return m_type; }

private:
    Type m_type;
    imController* m_controller;

    void allocController(unsigned int ctype);
};

class imAnimEvent {
public:
    imAnimEvent()
    { }

    ~imAnimEvent()
    { }

    void read(imStream* stream);
};

class imAnimBehavior {
public:
    imAnimBehavior() : m_saveFlags(0)
    { }

    ~imAnimBehavior()
    { }

    bool read(imStream* stream);

private:
    unsigned int m_saveFlags;
    float m_animTime;
    float m_frameRate;
    unsigned int m_frameCount;
    unsigned int m_firstFrame, m_lastFrame;
    std::vector<imAnimEvent> m_events;
};

#endif
