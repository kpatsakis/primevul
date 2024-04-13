datetime_s_weeknum(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vw, vd, vf, vh, vmin, vs, vof, vsg, y, fr, fr2, ret;
    int w, d, f, h, min, s, rof;
    double sg;

    rb_scan_args(argc, argv, "09", &vy, &vw, &vd, &vf,
		 &vh, &vmin, &vs, &vof, &vsg);

    y = INT2FIX(-4712);
    w = 0;
    d = 1;
    f = 0;

    h = min = s = 0;
    fr2 = INT2FIX(0);
    rof = 0;
    sg = DEFAULT_SG;

    switch (argc) {
      case 9:
	val2sg(vsg, sg);
      case 8:
	val2off(vof, rof);
      case 7:
	num2int_with_frac(s, positive_inf);
      case 6:
	num2int_with_frac(min, 6);
      case 5:
	num2int_with_frac(h, 5);
      case 4:
	f = NUM2INT(vf);
      case 3:
	num2int_with_frac(d, 4);
      case 2:
	w = NUM2INT(vw);
      case 1:
	y = vy;
    }

    {
	VALUE nth;
	int ry, rw, rd, rh, rmin, rs, rjd, rjd2, ns;

	if (!valid_weeknum_p(y, w, d, f, sg,
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