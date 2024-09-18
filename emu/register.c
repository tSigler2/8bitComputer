#include "register.h"

inline void updateRegister(struct Register* r, u8 val, Clock* c, bool reset){
    if(r->actLine && r->update == c->cs) r->val = val;
}