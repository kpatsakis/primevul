date_s_commercial(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vw, vd, vsg, y, fr, fr2, ret;
    int w, d;
    double sg;

    rb_scan_args(argc, argv, "04", &vy, &vw, &vd, &vsg);

    y = INT2FIX(-4712);
    w = 1;
    d = 1;
    fr2 = INT2FIX(0);
    sg = DEFAULT_SG;

    switch (argc) {
      case 4:
	val2sg(vsg, sg);
      case 3:
        check_numeric(vd, "cwday");
	num2int_with_frac(d, positive_inf);
      case 2:
        check_numeric(vw, "cweek");
	w = NUM2INT(vw);
      case 1:
        check_numeric(vy, "year");
	y = vy;
    }

    {
	VALUE nth;
	int ry, rw, rd, rjd, ns;

	if (!valid_commercial_p(y, w, d, sg,
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