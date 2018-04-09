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

#include "imSceneDatabase.h"
#include "imCommon.h"

/* imObjectDatabase */
bool imObjectDatabase::readIndex(imStream* stream)
{
    size_t count = stream->read32();
    for (size_t i=0; i<count; i++) {
        size_t slen = stream->read32();
        imString name = stream->readString(slen);

        std::vector<unsigned int> refs;
        refs.resize(stream->read32());
        for (size_t j=0; j<refs.size(); j++)
            refs[j] = stream->read32();
        m_objects[name] = refs;
    }
    return true;
}

std::vector<unsigned int> imObjectDatabase::get(imString name) const
{
    std::map<imString, std::vector<unsigned int> >::const_iterator f =
        m_objects.find(name);
    if (f == m_objects.end())
        return std::vector<unsigned int>();
    return f->second;
}


/* imSceneDatabase */
bool imSceneDatabase::readIndex(imStream* stream)
{
    if (stream->read32BE() != 3) {
        imLog("Fatal: Invalid Scene Database!\n");
        return false;
    }

    m_saveGroups.readIndex(stream);
    m_rooms.readIndex(stream);
    m_namedGroups.readIndex(stream);
    return true;
}
