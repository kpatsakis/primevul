fbCombineConjointGeneralC (CARD32 *dest, CARD32 *src, CARD32 *mask, int width, CARD8 combine)
{
    int i;

    for (i = 0; i < width; ++i) {
        CARD32  s, d;
        CARD32  m,n,o,p;
        CARD32  Fa, Fb;
        CARD16  t, u, v;
        CARD32  sa;
        CARD8   da;

        s = READ(src + i);
        m = READ(mask + i);
        d = READ(dest + i);
        da = d >> 24;

	fbCombineMaskC (&s, &m);

        sa = m;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            m = fbCombineConjointOutPart ((CARD8) (sa >> 0), da);
            n = fbCombineConjointOutPart ((CARD8) (sa >> 8), da) << 8;
            o = fbCombineConjointOutPart ((CARD8) (sa >> 16), da) << 16;
            p = fbCombineConjointOutPart ((CARD8) (sa >> 24), da) << 24;
            Fa = m|n|o|p;
            break;
        case CombineAIn:
            m = fbCombineConjointInPart ((CARD8) (sa >> 0), da);
            n = fbCombineConjointInPart ((CARD8) (sa >> 8), da) << 8;
            o = fbCombineConjointInPart ((CARD8) (sa >> 16), da) << 16;
            p = fbCombineConjointInPart ((CARD8) (sa >> 24), da) << 24;
            Fa = m|n|o|p;
            break;
        case CombineA:
            Fa = 0xffffffff;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            m = fbCombineConjointOutPart (da, (CARD8) (sa >> 0));
            n = fbCombineConjointOutPart (da, (CARD8) (sa >> 8)) << 8;
            o = fbCombineConjointOutPart (da, (CARD8) (sa >> 16)) << 16;
            p = fbCombineConjointOutPart (da, (CARD8) (sa >> 24)) << 24;
            Fb = m|n|o|p;
            break;
        case CombineBIn:
            m = fbCombineConjointInPart (da, (CARD8) (sa >> 0));
            n = fbCombineConjointInPart (da, (CARD8) (sa >> 8)) << 8;
            o = fbCombineConjointInPart (da, (CARD8) (sa >> 16)) << 16;
            p = fbCombineConjointInPart (da, (CARD8) (sa >> 24)) << 24;
            Fb = m|n|o|p;
            break;
        case CombineB:
            Fb = 0xffffffff;
            break;
        }
        m = FbGen (s,d,0,FbGet8(Fa,0),FbGet8(Fb,0),t, u, v);
        n = FbGen (s,d,8,FbGet8(Fa,8),FbGet8(Fb,8),t, u, v);
        o = FbGen (s,d,16,FbGet8(Fa,16),FbGet8(Fb,16),t, u, v);
        p = FbGen (s,d,24,FbGet8(Fa,24),FbGet8(Fb,24),t, u, v);
        s = m|n|o|p;
        WRITE(dest + i, s);
    }
}
