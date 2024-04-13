SourcePictureClassify (PicturePtr pict,
		       int	  x,
		       int	  y,
		       int	  width,
		       int	  height)
{
    if (pict->pSourcePict->type == SourcePictTypeSolidFill)
    {
	pict->pSourcePict->solidFill.class = SourcePictClassHorizontal;
    }
    else if (pict->pSourcePict->type == SourcePictTypeLinear)
    {
	PictVector   v;
	xFixed_32_32 l;
	xFixed_48_16 dx, dy, a, b, off;
	xFixed_48_16 factors[4];
	int	     i;

	dx = pict->pSourcePict->linear.p2.x - pict->pSourcePict->linear.p1.x;
	dy = pict->pSourcePict->linear.p2.y - pict->pSourcePict->linear.p1.y;
	l = dx * dx + dy * dy;
	if (l)
	{
	    a = (dx << 32) / l;
	    b = (dy << 32) / l;
	}
	else
	{
	    a = b = 0;
	}

	off = (-a * pict->pSourcePict->linear.p1.x
	       -b * pict->pSourcePict->linear.p1.y) >> 16;

	for (i = 0; i < 3; i++)
	{
	    v.vector[0] = IntToxFixed ((i % 2) * (width  - 1) + x);
	    v.vector[1] = IntToxFixed ((i / 2) * (height - 1) + y);
	    v.vector[2] = xFixed1;

	    if (pict->transform)
	    {
		if (!PictureTransformPoint3d (pict->transform, &v))
		    return SourcePictClassUnknown;
	    }

	    factors[i] = ((a * v.vector[0] + b * v.vector[1]) >> 16) + off;
	}

	if (factors[2] == factors[0])
	    pict->pSourcePict->linear.class = SourcePictClassHorizontal;
	else if (factors[1] == factors[0])
	    pict->pSourcePict->linear.class = SourcePictClassVertical;
    }

    return pict->pSourcePict->solidFill.class;
}
