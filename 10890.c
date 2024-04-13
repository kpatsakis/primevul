date_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE vy, vm, vd, vsg, y, fr, fr2, ret;
    int m, d;
    double sg;
    struct SimpleDateData *dat = rb_check_typeddata(self, &d_lite_type);

    if (!simple_dat_p(dat)) {
	rb_raise(rb_eTypeError, "Date expected");
    }

    rb_scan_args(argc, argv, "04", &vy, &vm, &vd, &vsg);

    y = INT2FIX(-4712);
    m = 1;
    d = 1;
    fr2 = INT2FIX(0);
    sg = DEFAULT_SG;

    switch (argc) {
      case 4:
	val2sg(vsg, sg);
      case 3:
        check_numeric(vd, "day");
	num2int_with_frac(d, positive_inf);
      case 2:
        check_numeric(vm, "month");
	m = NUM2INT(vm);
      case 1:
        check_numeric(vy, "year");
	y = vy;
    }

    if (guess_style(y, sg) < 0) {
	VALUE nth;
	int ry, rm, rd;

	if (!valid_gregorian_p(y, m, d,
			       &nth, &ry,
			       &rm, &rd))
	    rb_raise(eDateError, "invalid date");

	set_to_simple(self, dat, nth, 0, sg, ry, rm, rd, HAVE_CIVIL);
    }
    else {
	VALUE nth;
	int ry, rm, rd, rjd, ns;

	if (!valid_civil_p(y, m, d, sg,
			   &nth, &ry,
			   &rm, &rd, &rjd,
			   &ns))
	    rb_raise(eDateError, "invalid date");

	set_to_simple(self, dat, nth, rjd, sg, ry, rm, rd, HAVE_JD | HAVE_CIVIL);
    }
    ret = self;
    add_frac();
    return ret;
}