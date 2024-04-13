date_s__jisx0301(int argc, VALUE *argv, VALUE klass)
{
    VALUE str, opt;

    rb_scan_args(argc, argv, "1:", &str, &opt);
    check_limit(str, opt);

    return date__jisx0301(str);
}