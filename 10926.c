date_s_ordinal(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vd, vsg, y, fr, fr2, ret;
    int d;
    double sg;

    rb_scan_args(argc, argv, "03", &vy, &vd, &vsg);

    y = INT2FIX(-4712);
    d = 1;
    fr2 = INT2FIX(0);
    sg = DEFAULT_SG;

    switch (argc) {
      case 3:
	val2sg(vsg, sg);
      case 2:
        check_numeric(vd, "yday");
	num2int_with_frac(d, positive_inf);
      case 1:
        check_numeric(vy, "year");
	y = vy;
    }

    {
	VALUE nth;
	int ry, rd, rjd, ns;

	if (!valid_ordinal_p(y, d, sg,
			     &nth, &ry,
			     &rd, &rjd,
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