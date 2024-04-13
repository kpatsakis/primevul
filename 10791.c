old_to_new(VALUE ajd, VALUE of, VALUE sg,
	   VALUE *rnth, int *rjd, int *rdf, VALUE *rsf,
	   int *rof, double *rsg)
{
    VALUE jd, df, sf, of2, t;

    decode_day(f_add(ajd, half_days_in_day),
	       &jd, &df, &sf);
    t = day_to_sec(of);
    of2 = f_round(t);

    if (!f_eqeq_p(of2, t))
	rb_warning("fraction of offset is ignored");

    decode_jd(jd, rnth, rjd);

    *rdf = NUM2INT(df);
    *rsf = sf;
    *rof = NUM2INT(of2);
    *rsg = NUM2DBL(sg);

    if (*rdf < 0 || *rdf >= DAY_IN_SECONDS)
	rb_raise(eDateError, "invalid day fraction");

    if (f_lt_p(*rsf, INT2FIX(0)) ||
	f_ge_p(*rsf, INT2FIX(SECOND_IN_NANOSECONDS)))

    if (*rof < -DAY_IN_SECONDS || *rof > DAY_IN_SECONDS) {
	*rof = 0;
	rb_warning("invalid offset is ignored");
    }

    if (!c_valid_start_p(*rsg)) {
	*rsg = DEFAULT_SG;
	rb_warning("invalid start is ignored");
    }
}