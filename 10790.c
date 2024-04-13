valid_ordinal_p(VALUE y, int d, double sg,
		VALUE *nth, int *ry,
		int *rd, int *rjd,
		int *ns)
{
    double style = guess_style(y, sg);
    int r;

    if (style == 0) {
	int jd;

	r = c_valid_ordinal_p(FIX2INT(y), d, sg, rd, &jd, ns);
	if (!r)
	    return 0;
	decode_jd(INT2FIX(jd), nth, rjd);
	if (f_zero_p(*nth))
	    *ry = FIX2INT(y);
	else {
	    VALUE nth2;
	    decode_year(y, *ns ? -1 : +1, &nth2, ry);
	}
    }
    else {
	decode_year(y, style, nth, ry);
	r = c_valid_ordinal_p(*ry, d, style, rd, rjd, ns);
    }
    return r;
}