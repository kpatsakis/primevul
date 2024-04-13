valid_jd_sub(int argc, VALUE *argv, VALUE klass, int need_jd)
{
    double sg = NUM2DBL(argv[1]);
    valid_sg(sg);
    return argv[0];
}