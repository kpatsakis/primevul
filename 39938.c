static inline void low_nmcpy(unsigned char *dst, char *src)
{
       memcpy(&dst[8], src, 8);
}
