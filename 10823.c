rt_rewrite_frags(VALUE hash)
{
    VALUE seconds;

    seconds = del_hash("seconds");
    if (!NIL_P(seconds)) {
	VALUE offset, d, h, min, s, fr;

	offset = ref_hash("offset");
	if (!NIL_P(offset))
	    seconds = f_add(seconds, offset);

	d = f_idiv(seconds, INT2FIX(DAY_IN_SECONDS));
	fr = f_mod(seconds, INT2FIX(DAY_IN_SECONDS));

	h = f_idiv(fr, INT2FIX(HOUR_IN_SECONDS));
	fr = f_mod(fr, INT2FIX(HOUR_IN_SECONDS));

	min = f_idiv(fr, INT2FIX(MINUTE_IN_SECONDS));
	fr = f_mod(fr, INT2FIX(MINUTE_IN_SECONDS));

	s = f_idiv(fr, INT2FIX(1));
	fr = f_mod(fr, INT2FIX(1));

	set_hash("jd", f_add(UNIX_EPOCH_IN_CJD, d));
	set_hash("hour", h);
	set_hash("min", min);
	set_hash("sec", s);
	set_hash("sec_fraction", fr);
    }
    return hash;
}