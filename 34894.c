jbig2_get_int32(const byte *bptr)
{
    return ((int32_t) get_int16(bptr) << 16) | get_uint16(bptr + 2);
}
