jd_to_nth_kday(VALUE jd, double sg,
	       VALUE *nth, int *rjd,
	       int *ry, int *rm, int *rn, int *rk)
{
    decode_jd(jd, nth, rjd);
    c_jd_to_nth_kday(*rjd, sg, ry, rm, rn, rk);
}