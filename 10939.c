d_lite_plus(VALUE self, VALUE other)
{
    int try_rational = 1;
    get_d1(self);

  again:
    switch (TYPE(other)) {
      case T_FIXNUM:
	{
	    VALUE nth;
	    long t;
	    int jd;

	    nth = m_nth(dat);
	    t = FIX2LONG(other);
	    if (DIV(t, CM_PERIOD)) {
		nth = f_add(nth, INT2FIX(DIV(t, CM_PERIOD)));
		t = MOD(t, CM_PERIOD);
	    }

	    if (!t)
		jd = m_jd(dat);
	    else {
		jd = m_jd(dat) + (int)t;
		canonicalize_jd(nth, jd);
	    }

	    if (simple_dat_p(dat))
		return d_simple_new_internal(rb_obj_class(self),
					     nth, jd,
					     dat->s.sg,
					     0, 0, 0,
					     (dat->s.flags | HAVE_JD) &
					     ~HAVE_CIVIL);
	    else
		return d_complex_new_internal(rb_obj_class(self),
					      nth, jd,
					      dat->c.df, dat->c.sf,
					      dat->c.of, dat->c.sg,
					      0, 0, 0,
#ifndef USE_PACK
					      dat->c.hour,
					      dat->c.min,
					      dat->c.sec,
#else
					      EX_HOUR(dat->c.pc),
					      EX_MIN(dat->c.pc),
					      EX_SEC(dat->c.pc),
#endif
					      (dat->c.flags | HAVE_JD) &
					      ~HAVE_CIVIL);
	}
	break;
      case T_BIGNUM:
	{
	    VALUE nth;
	    int jd, s;

	    if (f_positive_p(other))
		s = +1;
	    else {
		s = -1;
		other = f_negate(other);
	    }

	    nth = f_idiv(other, INT2FIX(CM_PERIOD));
	    jd = FIX2INT(f_mod(other, INT2FIX(CM_PERIOD)));

	    if (s < 0) {
		nth = f_negate(nth);
		jd = -jd;
	    }

	    if (!jd)
		jd = m_jd(dat);
	    else {
		jd = m_jd(dat) + jd;
		canonicalize_jd(nth, jd);
	    }

	    if (f_zero_p(nth))
		nth = m_nth(dat);
	    else
		nth = f_add(m_nth(dat), nth);

	    if (simple_dat_p(dat))
		return d_simple_new_internal(rb_obj_class(self),
					     nth, jd,
					     dat->s.sg,
					     0, 0, 0,
					     (dat->s.flags | HAVE_JD) &
					     ~HAVE_CIVIL);
	    else
		return d_complex_new_internal(rb_obj_class(self),
					      nth, jd,
					      dat->c.df, dat->c.sf,
					      dat->c.of, dat->c.sg,
					      0, 0, 0,
#ifndef USE_PACK
					      dat->c.hour,
					      dat->c.min,
					      dat->c.sec,
#else
					      EX_HOUR(dat->c.pc),
					      EX_MIN(dat->c.pc),
					      EX_SEC(dat->c.pc),
#endif
					      (dat->c.flags | HAVE_JD) &
					      ~HAVE_CIVIL);
	}
	break;
      case T_FLOAT:
	{
	    double jd, o, tmp;
	    int s, df;
	    VALUE nth, sf;

	    o = RFLOAT_VALUE(other);

	    if (o > 0)
		s = +1;
	    else {
		s = -1;
		o = -o;
	    }

	    o = modf(o, &tmp);

	    if (!floor(tmp / CM_PERIOD)) {
		nth = INT2FIX(0);
		jd = (int)tmp;
	    }
	    else {
		double i, f;

		f = modf(tmp / CM_PERIOD, &i);
		nth = f_floor(DBL2NUM(i));
		jd = (int)(f * CM_PERIOD);
	    }

	    o *= DAY_IN_SECONDS;
	    o = modf(o, &tmp);
	    df = (int)tmp;
	    o *= SECOND_IN_NANOSECONDS;
	    sf = INT2FIX((int)round(o));

	    if (s < 0) {
		jd = -jd;
		df = -df;
		sf = f_negate(sf);
	    }

	    if (f_zero_p(sf))
		sf = m_sf(dat);
	    else {
		sf = f_add(m_sf(dat), sf);
		if (f_lt_p(sf, INT2FIX(0))) {
		    df -= 1;
		    sf = f_add(sf, INT2FIX(SECOND_IN_NANOSECONDS));
		}
		else if (f_ge_p(sf, INT2FIX(SECOND_IN_NANOSECONDS))) {
		    df += 1;
		    sf = f_sub(sf, INT2FIX(SECOND_IN_NANOSECONDS));
		}
	    }

	    if (!df)
		df = m_df(dat);
	    else {
		df = m_df(dat) + df;
		if (df < 0) {
		    jd -= 1;
		    df += DAY_IN_SECONDS;
		}
		else if (df >= DAY_IN_SECONDS) {
		    jd += 1;
		    df -= DAY_IN_SECONDS;
		}
	    }

	    if (!jd)
		jd = m_jd(dat);
	    else {
		jd = m_jd(dat) + jd;
		canonicalize_jd(nth, jd);
	    }

	    if (f_zero_p(nth))
		nth = m_nth(dat);
	    else
		nth = f_add(m_nth(dat), nth);

	    if (!df && f_zero_p(sf) && !m_of(dat))
		return d_simple_new_internal(rb_obj_class(self),
					     nth, (int)jd,
					     m_sg(dat),
					     0, 0, 0,
					     (dat->s.flags | HAVE_JD) &
					     ~(HAVE_CIVIL | HAVE_TIME |
					       COMPLEX_DAT));
	    else
		return d_complex_new_internal(rb_obj_class(self),
					      nth, (int)jd,
					      df, sf,
					      m_of(dat), m_sg(dat),
					      0, 0, 0,
					      0, 0, 0,
					      (dat->c.flags |
					       HAVE_JD | HAVE_DF) &
					      ~(HAVE_CIVIL | HAVE_TIME));
	}
	break;
      default:
	expect_numeric(other);
	other = f_to_r(other);
	if (!k_rational_p(other)) {
	    if (!try_rational) Check_Type(other, T_RATIONAL);
	    try_rational = 0;
	    goto again;
	}
	/* fall through */
      case T_RATIONAL:
	{
	    VALUE nth, sf, t;
	    int jd, df, s;

	    if (wholenum_p(other)) {
		other = rb_rational_num(other);
		goto again;
	    }

	    if (f_positive_p(other))
		s = +1;
	    else {
		s = -1;
		other = f_negate(other);
	    }

	    nth = f_idiv(other, INT2FIX(CM_PERIOD));
	    t = f_mod(other, INT2FIX(CM_PERIOD));

	    jd = FIX2INT(f_idiv(t, INT2FIX(1)));
	    t = f_mod(t, INT2FIX(1));

	    t = f_mul(t, INT2FIX(DAY_IN_SECONDS));
	    df = FIX2INT(f_idiv(t, INT2FIX(1)));
	    t = f_mod(t, INT2FIX(1));

	    sf = f_mul(t, INT2FIX(SECOND_IN_NANOSECONDS));

	    if (s < 0) {
		nth = f_negate(nth);
		jd = -jd;
		df = -df;
		sf = f_negate(sf);
	    }

	    if (f_zero_p(sf))
		sf = m_sf(dat);
	    else {
		sf = f_add(m_sf(dat), sf);
		if (f_lt_p(sf, INT2FIX(0))) {
		    df -= 1;
		    sf = f_add(sf, INT2FIX(SECOND_IN_NANOSECONDS));
		}
		else if (f_ge_p(sf, INT2FIX(SECOND_IN_NANOSECONDS))) {
		    df += 1;
		    sf = f_sub(sf, INT2FIX(SECOND_IN_NANOSECONDS));
		}
	    }

	    if (!df)
		df = m_df(dat);
	    else {
		df = m_df(dat) + df;
		if (df < 0) {
		    jd -= 1;
		    df += DAY_IN_SECONDS;
		}
		else if (df >= DAY_IN_SECONDS) {
		    jd += 1;
		    df -= DAY_IN_SECONDS;
		}
	    }

	    if (!jd)
		jd = m_jd(dat);
	    else {
		jd = m_jd(dat) + jd;
		canonicalize_jd(nth, jd);
	    }

	    if (f_zero_p(nth))
		nth = m_nth(dat);
	    else
		nth = f_add(m_nth(dat), nth);

	    if (!df && f_zero_p(sf) && !m_of(dat))
		return d_simple_new_internal(rb_obj_class(self),
					     nth, jd,
					     m_sg(dat),
					     0, 0, 0,
					     (dat->s.flags | HAVE_JD) &
					     ~(HAVE_CIVIL | HAVE_TIME |
					       COMPLEX_DAT));
	    else
		return d_complex_new_internal(rb_obj_class(self),
					      nth, jd,
					      df, sf,
					      m_of(dat), m_sg(dat),
					      0, 0, 0,
					      0, 0, 0,
					      (dat->c.flags |
					       HAVE_JD | HAVE_DF) &
					      ~(HAVE_CIVIL | HAVE_TIME));
	}
	break;
    }
}