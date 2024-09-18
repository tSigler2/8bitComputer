#include "register.h"

inline void updateRegister(struct Register* r, u8 val, Clock* c, bool reset){
    if(r->actLine && r->update == c->cs) r->val = val;
}

inline void asyncRest(struct Register* r){
    r->val = 0x00;
}