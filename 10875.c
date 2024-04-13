d_lite_downto(VALUE self, VALUE min)
{
    VALUE date;

    RETURN_ENUMERATOR(self, 1, &min);

    date = self;
    while (FIX2INT(d_lite_cmp(date, min)) >= 0) {
	rb_yield(date);
	date = d_lite_plus(date, INT2FIX(-1));
    }
    return self;
}