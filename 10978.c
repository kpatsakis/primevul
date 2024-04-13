canonicalize_s_jd(VALUE obj, union DateData *x)
{
    int j = x->s.jd;
    VALUE nth = x->s.nth;
    assert(have_jd_p(x));
    canonicalize_jd(nth, x->s.jd);
    RB_OBJ_WRITE(obj, &x->s.nth, nth);
    if (x->s.jd != j)
	x->flags &= ~HAVE_CIVIL;
}