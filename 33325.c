fbCombineDisjointOutPart (CARD8 a, CARD8 b)
{
    /* min (1, (1-b) / a) */

    b = ~b;		    /* 1 - b */
    if (b >= a)		    /* 1 - b >= a -> (1-b)/a >= 1 */
	return 0xff;	    /* 1 */
    return FbIntDiv(b,a);   /* (1-b) / a */
}
