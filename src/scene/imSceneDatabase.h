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

#ifndef _IM_SCENEDATABASE_H
#define _IM_SCENEDATABASE_H

#include <map>
#include <vector>
#include "../vfs/imStream.h"

class imObjectDatabase {
public:
    imObjectDatabase()
    { }

    ~imObjectDatabase()
    { }

    bool readIndex(imStream* stream);
    std::vector<unsigned int> get(imString name) const;

private:
    std::map<imString, std::vector<unsigned int> > m_objects;
};

class imSceneDatabase {
public:
    imSceneDatabase()
    { }

    ~imSceneDatabase()
    { }

    bool readIndex(imStream* stream);

    std::vector<unsigned int> saveGroup(imString name)
    { return m_saveGroups.get(name); }

    std::vector<unsigned int> room(imString name)
    { return m_rooms.get(name); }

    std::vector<unsigned int> namedGroup(imString name)
    { return m_namedGroups.get(name); }

private:
    imObjectDatabase m_saveGroups;
    imObjectDatabase m_rooms;
    imObjectDatabase m_namedGroups;
};

#endif
