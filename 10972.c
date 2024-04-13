jd_to_ordinal(VALUE jd, double sg,
	      VALUE *nth, int *rjd,
	      int *ry, int *rd)
{
    decode_jd(jd, nth, rjd);
    c_jd_to_ordinal(*rjd, sg, ry, rd);
}