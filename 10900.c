jd_to_civil(VALUE jd, double sg,
	    VALUE *nth, int *rjd,
	    int *ry, int *rm, int *rd)
{
    decode_jd(jd, nth, rjd);
    c_jd_to_civil(*rjd, sg, ry, rm, rd);
}