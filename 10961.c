d_lite_s_alloc_simple(VALUE klass)
{
    return d_simple_new_internal(klass,
				 INT2FIX(0), 0,
				 DEFAULT_SG,
				 0, 0, 0,
				 HAVE_JD);
}