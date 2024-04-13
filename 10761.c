d_lite_step(int argc, VALUE *argv, VALUE self)
{
    VALUE limit, step, date;
    int c;

    rb_scan_args(argc, argv, "11", &limit, &step);

    if (argc < 2)
	step = INT2FIX(1);

#if 0
    if (f_zero_p(step))
	rb_raise(rb_eArgError, "step can't be 0");
#endif

    RETURN_ENUMERATOR(self, argc, argv);

    date = self;
    c = f_cmp(step, INT2FIX(0));
    if (c < 0) {
	while (FIX2INT(d_lite_cmp(date, limit)) >= 0) {
	    rb_yield(date);
	    date = d_lite_plus(date, step);
	}
    }
    else if (c == 0) {
	while (1)
	    rb_yield(date);
    }
    else /* if (c > 0) */ {
	while (FIX2INT(d_lite_cmp(date, limit)) <= 0) {
	    rb_yield(date);
	    date = d_lite_plus(date, step);
	}
    }
    return self;
}