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
