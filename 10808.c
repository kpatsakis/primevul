date_s__valid_ordinal_p(int argc, VALUE *argv, VALUE klass)
{
    VALUE vy, vd, vsg;
    VALUE argv2[3];

    rb_scan_args(argc, argv, "21", &vy, &vd, &vsg);

    argv2[0] = vy;
    argv2[1] = vd;
    if (argc < 3)
	argv2[2] = DBL2NUM(GREGORIAN);
    else
	argv2[2] = vsg;

    return valid_ordinal_sub(3, argv2, klass, 1);
}