valid_gregorian_p(VALUE y, int m, int d,
		  VALUE *nth, int *ry,
		  int *rm, int *rd)
{
    decode_year(y, -1, nth, ry);
    return c_valid_gregorian_p(*ry, m, d, rm, rd);
}