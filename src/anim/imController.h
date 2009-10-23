#ifndef _IM_CONTROLLER_H
#define _IM_CONTROLLER_H

#include "../vfs/imStream.h"

class imController {
public:
    enum Type {
        kNoController, kSimpleController, kCompoundController
    };

    imController()
    { }

    virtual ~imController()
    { }

    virtual void read(imStream* stream) = 0;
};

class imSimplePosController : public imController {
public:
    imSimplePosController() : m_position(0)
    { }

    virtual ~imSimplePosController();

    virtual void read(imStream* stream);

private:
    class imPoint3Controller* m_position;
};

class imCompoundPosController : public imController {
public:
    imCompoundPosController()
    : m_XController(0), m_YController(0), m_ZController(0)
    { }

    virtual ~imCompoundPosController();

    virtual void read(imStream* stream);

private:
    class imScalarController* m_XController;
    class imScalarController* m_YController;
    class imScalarController* m_ZController;
};

#endif
