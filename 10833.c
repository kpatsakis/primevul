date_s_test_nth_kday(VALUE klass)
{
    if (!test_nth_kday(MIN_JD, MIN_JD + 366, GREGORIAN))
	return Qfalse;
    if (!test_nth_kday(2305814, 2598007, GREGORIAN))
	return Qfalse;
    if (!test_nth_kday(MAX_JD - 366, MAX_JD, GREGORIAN))
	return Qfalse;

    if (!test_nth_kday(MIN_JD, MIN_JD + 366, ITALY))
	return Qfalse;
    if (!test_nth_kday(2305814, 2598007, ITALY))
	return Qfalse;
    if (!test_nth_kday(MAX_JD - 366, MAX_JD, ITALY))
	return Qfalse;

    return Qtrue;
}