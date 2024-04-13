date_s_test_commercial(VALUE klass)
{
    if (!test_commercial(MIN_JD, MIN_JD + 366, GREGORIAN))
	return Qfalse;
    if (!test_commercial(2305814, 2598007, GREGORIAN))
	return Qfalse;
    if (!test_commercial(MAX_JD - 366, MAX_JD, GREGORIAN))
	return Qfalse;

    if (!test_commercial(MIN_JD, MIN_JD + 366, ITALY))
	return Qfalse;
    if (!test_commercial(2305814, 2598007, ITALY))
	return Qfalse;
    if (!test_commercial(MAX_JD - 366, MAX_JD, ITALY))
	return Qfalse;

    return Qtrue;
}