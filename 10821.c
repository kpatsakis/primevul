d_lite_lshift(VALUE self, VALUE other)
{
    expect_numeric(other);
    return d_lite_rshift(self, f_negate(other));
}