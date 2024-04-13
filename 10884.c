d_lite_gc_mark(void *ptr)
{
    union DateData *dat = ptr;
    if (simple_dat_p(dat))
	rb_gc_mark(dat->s.nth);
    else {
	rb_gc_mark(dat->c.nth);
	rb_gc_mark(dat->c.sf);
    }
}