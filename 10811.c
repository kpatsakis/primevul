dup_obj_with_new_start(VALUE obj, double sg)
{
    volatile VALUE dup = dup_obj(obj);
    {
	get_d1(dup);
	set_sg(dat, sg);
    }
    return dup;
}