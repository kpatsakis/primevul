static inline uint8_t sr(VGACommonState *s, int idx)
{
    return vbe_enabled(s) ? s->sr_vbe[idx] : s->sr[idx];
}
