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
