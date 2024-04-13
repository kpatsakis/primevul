datetime_s_commercial(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vw, vd, vh, vmin, vs, vof, vsg, y, fr, fr2, ret;
    int w, d, h, min, s, rof;
    double sg;

    rb_scan_args(argc, argv, "08", &vy, &vw, &vd, &vh, &vmin, &vs, &vof, &vsg);

    y = INT2FIX(-4712);
    w = 1;
    d = 1;

    h = min = s = 0;
    fr2 = INT2FIX(0);
    rof = 0;
    sg = DEFAULT_SG;

    switch (argc) {
      case 8:
	val2sg(vsg, sg);
      case 7:
	val2off(vof, rof);
      case 6:
        check_numeric(vs, "second");
	num2int_with_frac(s, positive_inf);
      case 5:
        check_numeric(vmin, "minute");
	num2int_with_frac(min, 5);
      case 4:
        check_numeric(vh, "hour");
	num2int_with_frac(h, 4);
      case 3:
        check_numeric(vd, "cwday");
	num2int_with_frac(d, 3);
      case 2:
        check_numeric(vw, "cweek");
	w = NUM2INT(vw);
      case 1:
        check_numeric(vy, "year");
	y = vy;
    }

    {
	VALUE nth;
	int ry, rw, rd, rh, rmin, rs, rjd, rjd2, ns;

	if (!valid_commercial_p(y, w, d, sg,
				&nth, &ry,
				&rw, &rd, &rjd,
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