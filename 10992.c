datetime_s_ordinal(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vd, vh, vmin, vs, vof, vsg, y, fr, fr2, ret;
    int d, h, min, s, rof;
    double sg;

    rb_scan_args(argc, argv, "07", &vy, &vd, &vh, &vmin, &vs, &vof, &vsg);

    y = INT2FIX(-4712);
    d = 1;

    h = min = s = 0;
    fr2 = INT2FIX(0);
    rof = 0;
    sg = DEFAULT_SG;

    switch (argc) {
      case 7:
	val2sg(vsg, sg);
      case 6:
	val2off(vof, rof);
      case 5:
        check_numeric(vs, "second");
	num2int_with_frac(s, positive_inf);
      case 4:
        check_numeric(vmin, "minute");
	num2int_with_frac(min, 4);
      case 3:
        check_numeric(vh, "hour");
	num2int_with_frac(h, 3);
      case 2:
        check_numeric(vd, "yday");
	num2int_with_frac(d, 2);
      case 1:
        check_numeric(vy, "year");
	y = vy;
    }

    {
	VALUE nth;
	int ry, rd, rh, rmin, rs, rjd, rjd2, ns;

	if (!valid_ordinal_p(y, d, sg,
			     &nth, &ry,
			     &rd, &rjd,
			     &ns))
	    rb_raise(eDateError, "invalid date");
	if (!c_valid_time_p(h, min, s, &rh, &rmin, &rs))
	    rb_raise(eDateError, "invalid date");
	canon24oc();

	rjd2 = jd_local_to_utc(rjd,
			       time_to_df(rh, rmin, rs),
			       rof);

	ret = d_complex_new_internal(klass,
				     nth, rjd2,
				     0, INT2FIX(0),
				     rof, sg,
				     0, 0, 0,
				     rh, rmin, rs,
				     HAVE_JD | HAVE_TIME);
    }
    add_frac();
    return ret;
}