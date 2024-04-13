dt_lite_iso8601(int argc, VALUE *argv, VALUE self)
{
    long n = 0;

    rb_check_arity(argc, 0, 1);
    if (argc >= 1)
	n = NUM2LONG(argv[0]);

    return rb_str_append(strftimev("%Y-%m-%d", self, set_tmx),
			 iso8601_timediv(self, n));
}