#pragma once
#include "../state.h"

template <typename _Object>
class CStateBloodsucksocVampire : public CState<_Object>
{
    typedef CState<_Object> inherited;
    typedef CState<_Object>* state_ptr;

    const CEntityAlive* enemy;

public:
    CStateBloodsucksocVampire(_Object* obj);

    virtual void reinit();

    virtual void initialize();
    virtual void reselect_state();
    virtual void finalize();
    virtual void critical_finalize();
    virtual bool check_start_conditions();
    virtual bool check_completion();
    virtual void remove_links(CObject* object);

    virtual void setup_substates();
    virtual void check_force_state();
};

#include "bloodsucksoc_vampire_inline.h"
