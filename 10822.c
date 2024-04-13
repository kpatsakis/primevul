date_s__parse_internal(int argc, VALUE *argv, VALUE klass)
{
    VALUE vstr, vcomp, hash, opt;

    rb_scan_args(argc, argv, "11:", &vstr, &vcomp, &opt);
    if (!NIL_P(opt)) argc--;
    check_limit(vstr, opt);
    StringValue(vstr);
    if (!rb_enc_str_asciicompat_p(vstr))
	rb_raise(rb_eArgError,
		 "string should have ASCII compatible encoding");
    if (argc < 2)
	vcomp = Qtrue;

    hash = date__parse(vstr, vcomp);

    return hash;
}