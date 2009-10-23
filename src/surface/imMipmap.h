#ifndef _IM_MIPMAP_H
#define _IM_MIPMAP_H

#include "../vfs/imStream.h"
#include "../imCommon.h"
#include <vector>

enum DXTLevel { kDXTInvalid, kDXT1, kDXT2, kDXT3, kDXT4, kDXT5 };

struct imMipLevel {
    unsigned int m_width, m_height;
    size_t m_offset, m_size;
};

class imMipmap {
public:
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
