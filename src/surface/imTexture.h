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

#ifndef _IM_TEXTURE_H
#define _IM_TEXTURE_H

#include "imKeyList.h"

class imTexture {
public:
    imTexture() : m_readFlags(), m_textureFlags() { }

    bool read(imStream* stream);

    const imKeyList& keys() const
    { return m_keys; }

    const ST::string& textureName() const
    { return m_textureName; }

    unsigned int flags() const
    { return m_textureFlags; }

private:
    imKeyList m_keys;
    unsigned int m_readFlags;
    ST::string m_textureName;
    unsigned int m_textureFlags;
};

#endif
