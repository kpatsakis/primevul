d_lite_s_alloc_complex(VALUE klass)
{
    return d_complex_new_internal(klass,
				  INT2FIX(0), 0,
				  0, INT2FIX(0),
				  0, DEFAULT_SG,
				  0, 0, 0,
				  0, 0, 0,
				  HAVE_JD | HAVE_DF);
}