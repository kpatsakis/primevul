jd_to_weeknum(VALUE jd, int f, double sg,
	      VALUE *nth, int *rjd,
	      int *ry, int *rw, int *rd)
{
    decode_jd(jd, nth, rjd);
    c_jd_to_weeknum(*rjd, f, sg, ry, rw, rd);
}