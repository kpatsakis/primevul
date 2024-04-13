valid_civil_p(VALUE y, int m, int d, double sg,
	      VALUE *nth, int *ry,
	      int *rm, int *rd, int *rjd,
	      int *ns)
{
    double style = guess_style(y, sg);
    int r;

    if (style == 0) {
	int jd;

	r = c_valid_civil_p(FIX2INT(y), m, d, sg, rm, rd, &jd, ns);
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
	if (style < 0)
	    r = c_valid_gregorian_p(*ry, m, d, rm, rd);
	else
	    r = c_valid_julian_p(*ry, m, d, rm, rd);
	if (!r)
	    return 0;
	c_civil_to_jd(*ry, *rm, *rd, style, rjd, ns);
    }
    return r;
}