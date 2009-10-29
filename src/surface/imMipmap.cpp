#include "imMipmap.h"
#include <squish.h>

bool imMipmap::s_noDXTCompression = false;

imMipmap::~imMipmap()
{
    if (m_buffer != 0)
        delete[] m_buffer;
    if (m_texId != 0xFFFFFFFF)
        glDeleteTextures(1, &m_texId);
}

void imMipmap::read(imStream* stream)
{
    unsigned int version = stream->read32();
    if (version != 1) {
        imLog("Invalid Mipmap version: %d", version);
        return;
    }
    imLog("DEBUG: [HSM] (+4) %d", stream->read32());
    m_height = stream->read32();
    m_width = stream->read32();
    imLog("DEBUG: [HSM] (+10) %d", stream->readByte());
    imLog("DEBUG: [HSM] (Compression Type) %d", stream->read32());

    m_levels.resize(stream->read32());
    imLog("DEBUG: [HSM] (+19) %d", stream->read32());
    m_stride = stream->read32();
    m_dxtType = (DXTLevel)stream->read32();
    imLog("DEBUG: [HSM] (DXT Type) %d", m_dxtType);
    buildMipLevels();

    if (m_buffer != 0)
        delete[] m_buffer;
    m_buffer = new unsigned char[m_totalSize];
    if (stream->tell() + m_totalSize != stream->size())
        imLog("WARN: HSM data size mismatch!  [%08X %08X]",
              stream->tell() + m_totalSize, stream->size());
    stream->read(m_buffer, m_totalSize);
}

void imMipmap::prepare()
{
    if (m_texId != 0xFFFFFFFF)
        glDeleteTextures(1, &m_texId);
    glGenTextures(1, &m_texId);

    bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    unsigned int dxtLevel = 0;
    if (s_noDXTCompression) {
        if (m_dxtType == kDXT1)
            dxtLevel = squish::kDxt1;
        else if (m_dxtType == kDXT3)
            dxtLevel = squish::kDxt3;
        else if (m_dxtType == kDXT5)
            dxtLevel = squish::kDxt5;
    } else {
        if (m_dxtType == kDXT1)
            dxtLevel = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        else if (m_dxtType == kDXT3)
            dxtLevel = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        else if (m_dxtType == kDXT5)
            dxtLevel = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    }

    for (size_t i=0; i<m_levels.size(); i++) {
        //imLog("Decompressing Level %u: %dx%d (0x%08x bytes) at 0x%08x",
        //      i, m_levels[i].m_width, m_levels[i].m_height, m_levels[i].m_size,
        //      m_levels[i].m_offset);
        if (s_noDXTCompression) {
            size_t fullsize = m_levels[i].m_width * m_levels[i].m_height * 4;
            squish::u8* buffer = new squish::u8[fullsize];
            squish::DecompressImage(buffer, m_levels[i].m_width, m_levels[i].m_height,
                                    m_buffer + m_levels[i].m_offset, dxtLevel);
            glTexImage2D(GL_TEXTURE_2D, i, GL_RGBA, m_levels[i].m_width,
                        m_levels[i].m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            delete[] buffer;
        } else {
            GLX_CompressedTexImage2D(GL_TEXTURE_2D, i, dxtLevel, m_levels[i].m_width,
                                     m_levels[i].m_height, 0, m_levels[i].m_size,
                                     m_buffer + m_levels[i].m_offset);
        }
    }
}

void imMipmap::buildMipLevels()
{
    unsigned int width = m_width;
    unsigned int height = m_height;
    m_totalSize = 0;
    for (size_t i=0; i<m_levels.size(); i++) {
        imMipLevel lev;
        lev.m_width = width;
        lev.m_height = height;
        lev.m_offset = m_totalSize;
        if (((width | height) & 3) != 0)
            lev.m_size = (height * width * 4);
        else
            lev.m_size = (height * width * m_stride) / 16;

        //imLog("%dx%d 0x%08x", width, height, lev.m_size);

        m_levels[i] = lev;
        m_totalSize += lev.m_size;
        if (width > 1)
            width /= 2;
        if (height > 1)
            height /= 2;
    }
}

void imMipmap::TEST_ExportDDS(imString filename)
{
    imFileStream S;
    S.open(filename, "wb");
    S.write("DDS ", 4);
    S.write32(124);
    S.write32(0x000A1007);
    S.write32(m_height);
    S.write32(m_width);
    S.write32((m_height * m_width * m_stride) / 16);
    S.write32(0);
    S.write32(m_levels.size() - 2);
    for (size_t i=0; i<11; i++)
        S.write32(0);

    S.write32(32);
    S.write32(4);
    if (m_dxtType == kDXT1)
        S.write("DXT1", 4);
    if (m_dxtType == kDXT3)
        S.write("DXT3", 4);
    if (m_dxtType == kDXT5)
        S.write("DXT5", 4);
    S.write32(0);
    S.write32(0);
    S.write32(0);
    S.write32(0);
    S.write32(0);
    S.write32(0x00401008);
    S.write32(0);
    S.write32(0);
    S.write32(0);
    S.write32(0);
    S.write(m_buffer, m_totalSize - 20);
}
