date_s_civil(int argc, VALUE *argv, VALUE klass)
{
    return date_initialize(argc, argv, d_lite_s_alloc_simple(klass));
}