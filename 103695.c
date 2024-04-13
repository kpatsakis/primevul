static uint32_t rotl(uint32_t v, unsigned count)
{
    count &= 31;
    return (v << count) | (v >> (32 - count));
}
