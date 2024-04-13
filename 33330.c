fbCombineDisjointOverU (CARD32 *dest, const CARD32 *src, int width)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32  s = READ(src + i);
        CARD16  a = s >> 24;

        if (a != 0x00)
        {
            if (a != 0xff)
            {
                CARD32 d = READ(dest + i);
                a = fbCombineDisjointOutPart (d >> 24, a);
                FbByteMulAdd(d, a, s);
                s = d;
            }
            WRITE(dest + i, s);
        }
    }
}
