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

#ifndef _IM_ANIMLAYER_H
#define _IM_ANIMLAYER_H

#include "imLayer.h"

class imAnimLayer : public imLayer {
public:
    imAnimLayer(unsigned int type)
    : imLayer(type), m_frameController(imAnimController::kTypeScalar)
    { }

    virtual ~imAnimLayer()
    { }

    virtual bool read(imStream* stream);
    virtual void readTextures(imStream* stream, class imSceneIndex* idx);

private:
    unsigned int m_frameCount;
    imAnimController m_frameController;
};

class imAVILayer : public imLayer {
public:
    imAVILayer(unsigned int type) : imLayer(type)
    { }

    virtual ~imAVILayer()
    { }

    virtual bool read(imStream* stream);

private:
    unsigned int m_animFlags;
    imString m_filename;
};

class imQTLayer : public imLayer {
public:
    imQTLayer(unsigned int type) : imLayer(type)
    { }

    virtual ~imQTLayer()
    { }

    virtual bool read(imStream* stream);

private:
    unsigned int m_animFlags;
    imString m_filename;
};

class imBinkLayer : public imLayer {
public:
    imBinkLayer(unsigned int type) : imLayer(type)
    { }

    virtual ~imBinkLayer()
    { }

    virtual bool read(imStream* stream);

private:
    unsigned int m_animFlags;
    imString m_filename;
};

class imFireLayer : public imLayer {
public:
    imFireLayer(unsigned int type) : imLayer(type)
    { }

    virtual ~imFireLayer()
    { }
};

class imWaterLayer : public imLayer {
public:
    imWaterLayer(unsigned int type) : imLayer(type)
    { }

    virtual ~imWaterLayer()
    { }

    virtual bool read(imStream* stream);

private:
    unsigned int m_animFlags;
};

#endif
