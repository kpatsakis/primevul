static inline void lsi_set_phase(LSIState *s, int phase)
{
    s->sbcl &= ~PHASE_MASK;
    s->sbcl |= phase | LSI_SBCL_REQ;
    s->sstat1 = (s->sstat1 & ~PHASE_MASK) | phase;
}
