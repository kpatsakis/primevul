commercial_to_jd(VALUE y, int w, int d, double sg,
		 VALUE *nth, int *ry,
		 int *rjd,
		 int *ns)
{
    double style = guess_style(y, sg);

    if (style == 0) {
	int jd;

	c_commercial_to_jd(FIX2INT(y), w, d, sg, &jd, ns);
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
	c_commercial_to_jd(*ry, w, d, style, rjd, ns);
    }
}