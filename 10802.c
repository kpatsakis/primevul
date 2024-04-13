date_s_nth_kday(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vm, vn, vk, vsg, y, fr, fr2, ret;
    int m, n, k;
    double sg;

    rb_scan_args(argc, argv, "05", &vy, &vm, &vn, &vk, &vsg);

    y = INT2FIX(-4712);
    m = 1;
    n = 1;
    k = 1;
    fr2 = INT2FIX(0);
    sg = DEFAULT_SG;

    switch (argc) {
      case 5:
	val2sg(vsg, sg);
      case 4:
	num2int_with_frac(k, positive_inf);
      case 3:
	n = NUM2INT(vn);
      case 2:
	m = NUM2INT(vm);
      case 1:
	y = vy;
    }

    {
	VALUE nth;
	int ry, rm, rn, rk, rjd, ns;

	if (!valid_nth_kday_p(y, m, n, k, sg,
			      &nth, &ry,
			      &rm, &rn, &rk, &rjd,
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