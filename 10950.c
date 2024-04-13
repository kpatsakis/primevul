d_simple_new_internal(VALUE klass,
		      VALUE nth, int jd,
		      double sg,
		      int y, int m, int d,
		      unsigned flags)
{
    struct SimpleDateData *dat;
    VALUE obj;

    obj = TypedData_Make_Struct(klass, struct SimpleDateData,
				&d_lite_type, dat);
    set_to_simple(obj, dat, nth, jd, sg, y, m, d, flags);

    assert(have_jd_p(dat) || have_civil_p(dat));

    return obj;
}