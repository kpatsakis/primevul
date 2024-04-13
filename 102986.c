static inline uint32_t uint_clamp(uint32_t val, uint32_t vmin, uint32_t vmax)
{
    if (val < vmin) {
        return vmin;
    }
    if (val > vmax) {
        return vmax;
    }
    return val;
}
