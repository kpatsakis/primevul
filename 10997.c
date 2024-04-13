date_s_test_unit_conv(VALUE klass)
{
    if (!test_unit_v2v_iter(sec_to_day, day_to_sec))
	return Qfalse;
    if (!test_unit_v2v_iter(ms_to_sec, sec_to_ms))
	return Qfalse;
    if (!test_unit_v2v_iter(ns_to_day, day_to_ns))
	return Qfalse;
    if (!test_unit_v2v_iter(ns_to_sec, sec_to_ns))
	return Qfalse;
    return Qtrue;
}