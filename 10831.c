date_s_parse(int argc, VALUE *argv, VALUE klass)
{
    VALUE str, comp, sg, opt;

    rb_scan_args(argc, argv, "03:", &str, &comp, &sg, &opt);
    if (!NIL_P(opt)) argc--;

    switch (argc) {
      case 0:
	str = rb_str_new2("-4712-01-01");
      case 1:
	comp = Qtrue;
      case 2:
	sg = INT2FIX(DEFAULT_SG);
    }

    {
        int argc2 = 2;
	VALUE argv2[3];
        argv2[0] = str;
        argv2[1] = comp;
        if (!NIL_P(opt)) argv2[argc2++] = opt;
	VALUE hash = date_s__parse(argc2, argv2, klass);
	return d_new_by_frags(klass, hash, sg);
    }
}