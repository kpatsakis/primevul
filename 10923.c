d_lite_new_start(int argc, VALUE *argv, VALUE self)
{
    VALUE vsg;
    double sg;

    rb_scan_args(argc, argv, "01", &vsg);

    sg = DEFAULT_SG;
    if (argc >= 1)
	val2sg(vsg, sg);

    return dup_obj_with_new_start(self, sg);
}