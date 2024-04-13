date_s_valid_ordinal_p(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vd, vsg;
    VALUE argv2[3];

    rb_scan_args(argc, argv, "21", &vy, &vd, &vsg);

    RETURN_FALSE_UNLESS_NUMERIC(vy);
    RETURN_FALSE_UNLESS_NUMERIC(vd);
    argv2[0] = vy;
    argv2[1] = vd;
    if (argc < 3)
	argv2[2] = INT2FIX(DEFAULT_SG);
    else
	argv2[2] = vsg;

    if (NIL_P(valid_ordinal_sub(3, argv2, klass, 0)))
	return Qfalse;
    return Qtrue;
}