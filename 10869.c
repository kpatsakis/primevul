dup_obj(VALUE self)
{
    get_d1a(self);

    if (simple_dat_p(adat)) {
	VALUE new = d_lite_s_alloc_simple(rb_obj_class(self));
	{
	    get_d1b(new);
	    bdat->s = adat->s;
	    RB_OBJ_WRITTEN(new, Qundef, bdat->s.nth);
	    return new;
	}
    }
    else {
	VALUE new = d_lite_s_alloc_complex(rb_obj_class(self));
	{
	    get_d1b(new);
	    bdat->c = adat->c;
	    RB_OBJ_WRITTEN(new, Qundef, bdat->c.nth);
	    RB_OBJ_WRITTEN(new, Qundef, bdat->c.sf);
	    return new;
	}
    }
}