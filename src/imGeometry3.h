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

#ifndef _IM_GEOMETRY3_H
#define _IM_GEOMETRY3_H

#include "vfs/imStream.h"
#include <cstring>

struct imVector3 {
    imVector3()
    { }

    imVector3(float _x, float _y, float _z)
    : X(_x), Y(_y), Z(_z)
    { }

    float X, Y, Z;

    void read(imStream* stream)
    {
        X = stream->readFloat();
        Y = stream->readFloat();
        Z = stream->readFloat();
    }
};

struct imMatrix44 {
    imMatrix44()
    { }

    imMatrix44(float _11, float _12, float _13, float _14,
               float _21, float _22, float _23, float _24,
               float _31, float _32, float _33, float _34,
               float _41, float _42, float _43, float _44);

    float m_data[16];

    void read(imStream* stream);

    static const imMatrix44 IDENTITY;
};

#endif
