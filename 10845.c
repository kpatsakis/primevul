civil_to_jd(VALUE y, int m, int d, double sg,
	    VALUE *nth, int *ry,
	    int *rjd,
	    int *ns)
{
    double style = guess_style(y, sg);

    if (style == 0) {
	int jd;

	c_civil_to_jd(FIX2INT(y), m, d, sg, &jd, ns);
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
	c_civil_to_jd(*ry, m, d, style, rjd, ns);
    }
}