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

#ifndef _IM_KEYLIST_H
#define _IM_KEYLIST_H

#include "vfs/imStream.h"
#include <vector>

class imKey {
public:
    imKey() : m_type(0xFF)
    { }

    void read(imStream* stream);

    imString name() const
    { return m_name; }

    unsigned char type() const
    { return m_type; }

    bool isValid() const
    { return (m_type != 0xFF); }

private:
    imString m_name;
    unsigned char m_type;
};

class imKeyList {
public:
    imKeyList()
    { }

    void read(imStream* stream);

    const imKey& self() const
    { return m_self; }

    const std::vector<imKey>& refs() const
    { return m_refs; }

private:
    imKey m_self;
    std::vector<imKey> m_refs;
};

#endif
