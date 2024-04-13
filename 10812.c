d_lite_marshal_load(VALUE self, VALUE a)
{
    VALUE nth, sf;
    int jd, df, of;
    double sg;

    get_d1(self);

    rb_check_frozen(self);

    if (!RB_TYPE_P(a, T_ARRAY))
	rb_raise(rb_eTypeError, "expected an array");

    switch (RARRAY_LEN(a)) {
      case 2: /* 1.6.x */
      case 3: /* 1.8.x, 1.9.2 */
	{
	    VALUE ajd, vof, vsg;

	    if  (RARRAY_LEN(a) == 2) {
		ajd = f_sub(RARRAY_AREF(a, 0), half_days_in_day);
		vof = INT2FIX(0);
		vsg = RARRAY_AREF(a, 1);
		if (!k_numeric_p(vsg))
		    vsg = DBL2NUM(RTEST(vsg) ? GREGORIAN : JULIAN);
	    }
	    else {
		ajd = RARRAY_AREF(a, 0);
		vof = RARRAY_AREF(a, 1);
		vsg = RARRAY_AREF(a, 2);
	    }

	    old_to_new(ajd, vof, vsg,
		       &nth, &jd, &df, &sf, &of, &sg);
	}
	break;
      case 6:
	{
	    nth = RARRAY_AREF(a, 0);
	    jd = NUM2INT(RARRAY_AREF(a, 1));
	    df = NUM2INT(RARRAY_AREF(a, 2));
	    sf = RARRAY_AREF(a, 3);
	    of = NUM2INT(RARRAY_AREF(a, 4));
	    sg = NUM2DBL(RARRAY_AREF(a, 5));
	}
	break;
      default:
	rb_raise(rb_eTypeError, "invalid size");
	break;
    }

    if (simple_dat_p(dat)) {
	if (df || !f_zero_p(sf) || of) {
	    /* loading a fractional date; promote to complex */
	    dat = ruby_xrealloc(dat, sizeof(struct ComplexDateData));
	    RTYPEDDATA(self)->data = dat;
	    goto complex_data;
	}
	set_to_simple(self, &dat->s, nth, jd, sg, 0, 0, 0, HAVE_JD);
    } else {
      complex_data:
	set_to_complex(self, &dat->c, nth, jd, df, sf, of, sg,
		       0, 0, 0, 0, 0, 0,
		       HAVE_JD | HAVE_DF);
    }

    if (FL_TEST(a, FL_EXIVAR)) {
	rb_copy_generic_ivar(self, a);
	FL_SET(self, FL_EXIVAR);
    }

    return self;
}