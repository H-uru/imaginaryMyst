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
