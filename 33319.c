fbCombineDisjointGeneralU (CARD32 *dest, const CARD32 *src, int width, CARD8 combine)
{
    int i;
    for (i = 0; i < width; ++i) {
        CARD32 s = READ(src + i);
        CARD32 d = READ(dest + i);
        CARD32 m,n,o,p;
        CARD16 Fa, Fb, t, u, v;
        CARD8 sa = s >> 24;
        CARD8 da = d >> 24;

        switch (combine & CombineA) {
        default:
            Fa = 0;
            break;
        case CombineAOut:
            Fa = fbCombineDisjointOutPart (sa, da);
            break;
        case CombineAIn:
            Fa = fbCombineDisjointInPart (sa, da);
            break;
        case CombineA:
            Fa = 0xff;
            break;
        }

        switch (combine & CombineB) {
        default:
            Fb = 0;
            break;
        case CombineBOut:
            Fb = fbCombineDisjointOutPart (da, sa);
            break;
        case CombineBIn:
            Fb = fbCombineDisjointInPart (da, sa);
            break;
        case CombineB:
            Fb = 0xff;
            break;
        }
        m = FbGen (s,d,0,Fa,Fb,t, u, v);
        n = FbGen (s,d,8,Fa,Fb,t, u, v);
        o = FbGen (s,d,16,Fa,Fb,t, u, v);
        p = FbGen (s,d,24,Fa,Fb,t, u, v);
        s = m|n|o|p;
        WRITE(dest + i, s);
    }
}
