date_s_test_ordinal(VALUE klass)
{
    if (!test_ordinal(MIN_JD, MIN_JD + 366, GREGORIAN))
	return Qfalse;
    if (!test_ordinal(2305814, 2598007, GREGORIAN))
	return Qfalse;
    if (!test_ordinal(MAX_JD - 366, MAX_JD, GREGORIAN))
	return Qfalse;

    if (!test_ordinal(MIN_JD, MIN_JD + 366, ITALY))
	return Qfalse;
    if (!test_ordinal(2305814, 2598007, ITALY))
	return Qfalse;
    if (!test_ordinal(MAX_JD - 366, MAX_JD, ITALY))
	return Qfalse;

    return Qtrue;
}