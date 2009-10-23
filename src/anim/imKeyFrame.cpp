#include "imKeyFrame.h"

void imKeyFrame::read(imStream* stream)
{
    m_flags = stream->read32();
    m_frame = stream->read32();
    m_frameTime = stream->readFloat();
}

void imScalarKey::read(imStream* stream)
{
    imKeyFrame::read(stream);

    if ((flags() & kBezier) != 0) {
        m_tanIn = stream->readFloat();
        m_tanOut = stream->readFloat();
    }
    m_value = stream->readFloat();
}

void imPoint3Key::read(imStream* stream)
{
    imKeyFrame::read(stream);

    if ((flags() & kBezier) != 0) {
        m_tanIn.read(stream);
        m_tanOut.read(stream);
    }
    m_value.read(stream);
}

void imMatrix44Key::read(imStream* stream)
{
    imKeyFrame::read(stream);
    m_value.read(stream);
}
