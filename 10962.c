datetime_s_civil(int argc, VALUE *argv, VALUE klass)
{
    return datetime_initialize(argc, argv, d_lite_s_alloc_complex(klass));
}