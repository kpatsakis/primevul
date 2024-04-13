datetime_s_xmlschema(int argc, VALUE *argv, VALUE klass)
{
    VALUE str, sg, opt;

    rb_scan_args(argc, argv, "02:", &str, &sg, &opt);
    if (!NIL_P(opt)) argc--;

    switch (argc) {
      case 0:
	str = rb_str_new2("-4712-01-01T00:00:00+00:00");
      case 1:
	sg = INT2FIX(DEFAULT_SG);
    }

    {
        int argc2 = 1;
        VALUE argv2[2];
        argv2[0] = str;
        argv2[1] = opt;
        if (!NIL_P(opt)) argc2++;
	VALUE hash = date_s__xmlschema(argc2, argv2, klass);
	return dt_new_by_frags(klass, hash, sg);
    }
}