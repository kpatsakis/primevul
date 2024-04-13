date_s_valid_weeknum_p(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vw, vd, vf, vsg;
    VALUE argv2[5];

    rb_scan_args(argc, argv, "41", &vy, &vw, &vd, &vf, &vsg);

    argv2[0] = vy;
    argv2[1] = vw;
    argv2[2] = vd;
    argv2[3] = vf;
    if (argc < 5)
	argv2[4] = INT2FIX(DEFAULT_SG);
    else
	argv2[4] = vsg;

    if (NIL_P(valid_weeknum_sub(5, argv2, klass, 0)))
	return Qfalse;
    return Qtrue;
}