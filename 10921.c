canonicalize_c_jd(VALUE obj, union DateData *x)
{
    int j = x->c.jd;
    VALUE nth = x->c.nth;
    assert(have_jd_p(x));
    canonicalize_jd(nth, x->c.jd);
    RB_OBJ_WRITE(obj, &x->c.nth, nth);
    if (x->c.jd != j)
	x->flags &= ~HAVE_CIVIL;
}