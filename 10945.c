rt__valid_commercial_p(VALUE y, VALUE w, VALUE d, VALUE sg)
{
    VALUE nth, rjd2;
    int ry, rw, rd, rjd, ns;

    if (!valid_commercial_p(y, NUM2INT(w), NUM2INT(d), NUM2DBL(sg),
			    &nth, &ry,
			    &rw, &rd, &rjd,
			    &ns))
	return Qnil;
    encode_jd(nth, rjd, &rjd2);
    return rjd2;
}