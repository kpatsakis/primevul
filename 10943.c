d_new_by_frags(VALUE klass, VALUE hash, VALUE sg)
{
    VALUE jd;

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
	!NIL_P(ref_hash("mday")))
	jd = rt__valid_civil_p(ref_hash("year"),
			       ref_hash("mon"),
			       ref_hash("mday"), sg);
    else {
	hash = rt_rewrite_frags(hash);
	hash = rt_complete_frags(klass, hash);
	jd = rt__valid_date_frags_p(hash, sg);
    }

    if (NIL_P(jd))
	rb_raise(eDateError, "invalid date");
    {
	VALUE nth;
	int rjd;

	decode_jd(jd, &nth, &rjd);
	return d_simple_new_internal(klass,
				     nth, rjd,
				     NUM2DBL(sg),
				     0, 0, 0,
				     HAVE_JD);
    }
}