date_s_weeknum(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vw, vd, vf, vsg, y, fr, fr2, ret;
    int w, d, f;
    double sg;

    rb_scan_args(argc, argv, "05", &vy, &vw, &vd, &vf, &vsg);

    y = INT2FIX(-4712);
    w = 0;
    d = 1;
    f = 0;
    fr2 = INT2FIX(0);
    sg = DEFAULT_SG;

    switch (argc) {
      case 5:
	val2sg(vsg, sg);
      case 4:
	f = NUM2INT(vf);
      case 3:
	num2int_with_frac(d, positive_inf);
      case 2:
	w = NUM2INT(vw);
      case 1:
	y = vy;
    }

    {
	VALUE nth;
	int ry, rw, rd, rjd, ns;

	if (!valid_weeknum_p(y, w, d, f, sg,
			     &nth, &ry,
			     &rw, &rd, &rjd,
			     &ns))
	    rb_raise(eDateError, "invalid date");

	ret = d_simple_new_internal(klass,
				    nth, rjd,
				    sg,
				    0, 0, 0,
				    HAVE_JD);
    }
    add_frac();
    return ret;
}