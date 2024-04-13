rt__valid_weeknum_p(VALUE y, VALUE w, VALUE d, VALUE f, VALUE sg)
{
    VALUE nth, rjd2;
    int ry, rw, rd, rjd, ns;

    if (!valid_weeknum_p(y, NUM2INT(w), NUM2INT(d), NUM2INT(f), NUM2DBL(sg),
			 &nth, &ry,
			 &rw, &rd, &rjd,
			 &ns))
	return Qnil;
    encode_jd(nth, rjd, &rjd2);
    return rjd2;
}