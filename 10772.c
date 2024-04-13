valid_weeknum_p(VALUE y, int w, int d, int f, double sg,
		VALUE *nth, int *ry,
		int *rw, int *rd, int *rjd,
		int *ns)
{
    double style = guess_style(y, sg);
    int r;

    if (style == 0) {
	int jd;

	r = c_valid_weeknum_p(FIX2INT(y), w, d, f, sg, rw, rd, &jd, ns);
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
	r = c_valid_weeknum_p(*ry, w, d, f, style, rw, rd, rjd, ns);
    }
    return r;
}