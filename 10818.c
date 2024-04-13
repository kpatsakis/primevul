guess_style(VALUE y, double sg) /* -/+oo or zero */
{
    double style = 0;

    if (isinf(sg))
	style = sg;
    else if (!FIXNUM_P(y))
	style = f_positive_p(y) ? negative_inf : positive_inf;
    else {
	long iy = FIX2LONG(y);

	assert(FIXNUM_P(y));
	if (iy < REFORM_BEGIN_YEAR)
	    style = positive_inf;
	else if (iy > REFORM_END_YEAR)
	    style = negative_inf;
    }
    return style;
}