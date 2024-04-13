fbCombineSaturateC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width)
{
    int i;

    for (i = 0; i < width; ++i) {
        CARD32  s, d;
        CARD16  sa, sr, sg, sb, da;
        CARD16  t, u, v;
        CARD32  m,n,o,p;

        d = READ(dest + i);
        s = READ(src + i);
	m = READ(mask + i);

	fbCombineMaskC (&s, &m);

        sa = (m >> 24);
        sr = (m >> 16) & 0xff;
        sg = (m >>  8) & 0xff;
        sb = (m      ) & 0xff;
        da = ~d >> 24;

        if (sb <= da)
            m = FbAdd(s,d,0,t);
        else
            m = FbGen (s, d, 0, (da << 8) / sb, 0xff, t, u, v);

        if (sg <= da)
            n = FbAdd(s,d,8,t);
        else
            n = FbGen (s, d, 8, (da << 8) / sg, 0xff, t, u, v);

        if (sr <= da)
            o = FbAdd(s,d,16,t);
        else
            o = FbGen (s, d, 16, (da << 8) / sr, 0xff, t, u, v);

        if (sa <= da)
            p = FbAdd(s,d,24,t);
        else
            p = FbGen (s, d, 24, (da << 8) / sa, 0xff, t, u, v);

        WRITE(dest + i, m|n|o|p);
    }
}
