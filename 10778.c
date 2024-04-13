dup_obj_with_new_offset(VALUE obj, int of)
{
    volatile VALUE dup = dup_obj_as_complex(obj);
    {
	get_d1(dup);
	set_of(dat, of);
    }
    return dup;
}