expect_numeric(VALUE x)
{
    if (!k_numeric_p(x))
	rb_raise(rb_eTypeError, "expected numeric");
}