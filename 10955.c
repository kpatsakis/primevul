dup_obj_as_complex(VALUE self)
{
    get_d1a(self);

    if (simple_dat_p(adat)) {
	VALUE new = d_lite_s_alloc_complex(rb_obj_class(self));
	{
	    get_d1b(new);
	    copy_simple_to_complex(new, &bdat->c, &adat->s);
	    bdat->c.flags |= HAVE_DF | COMPLEX_DAT;
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