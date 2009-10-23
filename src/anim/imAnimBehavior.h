#ifndef _IM_ANIMBEHAVIOR_H
#define _IM_ANIMBEHAVIOR_H

#include <vector>
#include "../vfs/imStream.h"
#include "imController.h"

class imAnimController {
public:
    enum Type {
        kTypeInvalid, kTypeScalar, kTypeColor, kTypeMatrix44
    };

    imAnimController(Type type) : m_type(type), m_controller(0)
    { }

    ~imAnimController()
    {
        if (m_controller != 0)
            delete m_controller;
    }

    void read(imStream* stream);

    Type type() const
    { return m_type; }

private:
    Type m_type;
    imController* m_controller;

    void allocController(unsigned int ctype);
};

class imAnimEvent {
public:
    imAnimEvent()
    { }

    ~imAnimEvent()
    { }

    void read(imStream* stream);
};

class imAnimBehavior {
public:
    imAnimBehavior() : m_saveFlags(0)
    { }

    ~imAnimBehavior()
    { }

    bool read(imStream* stream);

private:
    unsigned int m_saveFlags;
    float m_animTime;
    float m_frameRate;
    unsigned int m_frameCount;
    unsigned int m_firstFrame, m_lastFrame;
    std::vector<imAnimEvent> m_events;
};

#endif
