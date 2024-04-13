jbig2_get_uint32(const byte *bptr)
{
    return ((uint32_t) get_uint16(bptr) << 16) | get_uint16(bptr + 2);
}
