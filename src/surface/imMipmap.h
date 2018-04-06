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

#ifndef _IM_MIPMAP_H
#define _IM_MIPMAP_H

#include "vfs/imStream.h"
#include "imCommon.h"
#include <vector>

enum DXTLevel { kDXTInvalid, kDXT1, kDXT2, kDXT3, kDXT4, kDXT5 };

struct imMipLevel {
    unsigned int m_width, m_height;
    size_t m_offset, m_size;
};

class imMipmap {
public:
    static bool s_noDXTCompression;

    imMipmap()
    : m_width(0), m_height(0), m_buffer(0), m_dxtType(kDXTInvalid),
      m_texId(0xFFFFFFFF)
    { }

    ~imMipmap();

    void read(imStream* stream);
    void prepare();

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, (GLuint)m_texId);
    }

    void TEST_ExportDDS(imString filename);

private:
    unsigned int m_width, m_height, m_stride;
    unsigned char* m_buffer;
    DXTLevel m_dxtType;
    std::vector<imMipLevel> m_levels;
    size_t m_totalSize;
    unsigned int m_texId;

    void buildMipLevels();
};

#endif
