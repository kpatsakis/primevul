static inline int lsi_irq_on_rsl(LSIState *s)
{
    return (s->sien0 & LSI_SIST0_RSL) && (s->scid & LSI_SCID_RRE);
}
