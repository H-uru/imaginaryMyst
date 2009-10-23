#ifndef _IM_LEAFCONTROLLER_H
#define _IM_LEAFCONTROLLER_H

#include "imController.h"
#include "imKeyFrame.h"
#include <vector>

class imLeafController : public imController {
public:
    imLeafController()
    { }

    virtual ~imLeafController();

    virtual void read(imStream* stream);

private:
    std::vector<class imEaseController*> m_ease;
};

class imEaseController : public imLeafController {
public:
    imEaseController()
    { }

    virtual ~imEaseController()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imScalarKey> m_keys;
};

class imScalarController : public imLeafController {
public:
    imScalarController()
    { }

    virtual ~imScalarController()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imScalarKey> m_keys;
};

class imPoint3Controller : public imLeafController {
public:
    imPoint3Controller()
    { }

    virtual ~imPoint3Controller()
    { }

    virtual void read(imStream* stream);

private:
    bool m_enabled;
    std::vector<imPoint3Key> m_keys;
};

class imMatrix44Controller : public imLeafController {
public:
    imMatrix44Controller()
    { }

    virtual ~imMatrix44Controller()
    { }

    virtual void read(imStream* stream);

private:
    std::vector<imMatrix44Key> m_keys;
};

#endif
