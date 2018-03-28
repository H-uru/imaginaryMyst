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

#include "imKeyList.h"

/* imKey */
void imKey::read(imStream* stream)
{
    size_t len = stream->read32();
    m_name = stream->readString(len);
    m_type = stream->readByte();
}


/* imKeyList */
void imKeyList::read(imStream* stream)
{
    if (stream->read32() != 0)
        m_self.read(stream);

    m_refs.resize(stream->read32());
    for (size_t i=0; i<m_refs.size(); i++)
        m_refs[i].read(stream);
}
