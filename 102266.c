encode_rfc1035(const char *src, uint8_t *dst)
{
 uint8_t *p = dst;
 uint8_t *lp = p++;

 if (*src == '\0')
 return 0;
 for (; *src; src++) {
 if (*src == '\0')
 break;
 if (*src == '.') {
 /* Skip the trailing . */
 if (src[1] == '\0')
 break;
 *lp = p - lp - 1;
 if (*lp == '\0')
 return p - dst;
			lp = p++;
 } else
 *p++ = (uint8_t)*src;
 }
 *lp = p - lp - 1;
 *p++ = '\0';
 return p - dst;
}
