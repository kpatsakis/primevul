dt_new_by_frags(VALUE klass, VALUE hash, VALUE sg)
{
    VALUE jd, sf, t;
    int df, of;

    if (!c_valid_start_p(NUM2DBL(sg))) {
	sg = INT2FIX(DEFAULT_SG);
	rb_warning("invalid start is ignored");
    }

    if (NIL_P(hash))
	rb_raise(eDateError, "invalid date");

    if (NIL_P(ref_hash("jd")) &&
	NIL_P(ref_hash("yday")) &&
	!NIL_P(ref_hash("year")) &&
	!NIL_P(ref_hash("mon")) &&
	!NIL_P(ref_hash("mday"))) {
	jd = rt__valid_civil_p(ref_hash("year"),
			       ref_hash("mon"),
			       ref_hash("mday"), sg);

	if (NIL_P(ref_hash("hour")))
	    set_hash("hour", INT2FIX(0));
	if (NIL_P(ref_hash("min")))
	    set_hash("min", INT2FIX(0));
	if (NIL_P(ref_hash("sec")))
	    set_hash("sec", INT2FIX(0));
	else if (f_eqeq_p(ref_hash("sec"), INT2FIX(60)))
	    set_hash("sec", INT2FIX(59));
    }
    else {
	hash = rt_rewrite_frags(hash);
	hash = rt_complete_frags(klass, hash);
	jd = rt__valid_date_frags_p(hash, sg);
    }

    if (NIL_P(jd))
	rb_raise(eDateError, "invalid date");

    {
	int rh, rmin, rs;

	if (!c_valid_time_p(NUM2INT(ref_hash("hour")),
			    NUM2INT(ref_hash("min")),
			    NUM2INT(ref_hash("sec")),
			    &rh, &rmin, &rs))
	    rb_raise(eDateError, "invalid date");

	df = time_to_df(rh, rmin, rs);
    }

    t = ref_hash("sec_fraction");
    if (NIL_P(t))
	sf = INT2FIX(0);
    else
	sf = sec_to_ns(t);

    t = ref_hash("offset");
    if (NIL_P(t))
	of = 0;
    else {
	of = NUM2INT(t);
	if (of < -DAY_IN_SECONDS || of > DAY_IN_SECONDS) {
	    of = 0;
	    rb_warning("invalid offset is ignored");
	}
    }
    {
	VALUE nth;
	int rjd, rjd2;

	decode_jd(jd, &nth, &rjd);
	rjd2 = jd_local_to_utc(rjd, df, of);
	df = df_local_to_utc(df, of);

	return d_complex_new_internal(klass,
				      nth, rjd2,
				      df, sf,
				      of, NUM2DBL(sg),
				      0, 0, 0,
				      0, 0, 0,
				      HAVE_JD | HAVE_DF);
    }
}