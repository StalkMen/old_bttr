#pragma once
#include "../state.h"

template <typename _Object>
class CStateBloodsucksocVampireApproach : public CState<_Object>
{
    typedef CState<_Object> inherited;

public:
    CStateBloodsucksocVampireApproach(_Object* obj);
    virtual ~CStateBloodsucksocVampireApproach();

    virtual void initialize();
    virtual void execute();
    virtual void remove_links(CObject* object) { inherited::remove_links(object); }
};

#include "bloodsucksoc_vampire_approach_inline.h"
