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

#ifndef _IM_KEYFRAME_H
#define _IM_KEYFRAME_H

#include "vfs/imStream.h"
#include "imGeometry3.h"

class imKeyFrame {
public:
    enum {
        kBezier = (1<<1),
    };

    imKeyFrame()
    { }

    virtual void read(imStream* stream);

    unsigned int flags() const
    { return m_flags; }

    unsigned int frame() const
    { return m_frame; }

    float frameTime() const
    { return m_frameTime; }

private:
    unsigned int m_flags;
    unsigned int m_frame;
    float m_frameTime;
};

class imScalarKey : public imKeyFrame {
public:
    imScalarKey()
    { }

    virtual void read(imStream* stream);

    float tanIn() const
    { return m_tanIn; }

    float tanOut() const
    { return m_tanOut; }

    float value() const
    { return m_value; }

private:
    float m_tanIn, m_tanOut;
    float m_value;
};

class imPoint3Key : public imKeyFrame {
public:
    imPoint3Key()
    { }

    virtual void read(imStream* stream);

    const imVector3& tanIn() const
    { return m_tanIn; }

    const imVector3& tanOut() const
    { return m_tanOut; }

    const imVector3& value() const
    { return m_value; }

private:
    imVector3 m_tanIn, m_tanOut;
    imVector3 m_value;
};

class imMatrix44Key : public imKeyFrame {
public:
    imMatrix44Key()
    { }

    virtual void read(imStream* stream);

    const imMatrix44& value() const
    { return m_value; }

private:
    imMatrix44 m_value;
};

#endif
