datetime_s_now(int argc, VALUE *argv, VALUE klass)
{
    VALUE vsg, nth, ret;
    double sg;
#ifdef HAVE_CLOCK_GETTIME
    struct timespec ts;
#else
    struct timeval tv;
#endif
    time_t sec;
    struct tm tm;
    long sf, of;
    int y, ry, m, d, h, min, s;

    rb_scan_args(argc, argv, "01", &vsg);

    if (argc < 1)
	sg = DEFAULT_SG;
    else
	sg = NUM2DBL(vsg);

#ifdef HAVE_CLOCK_GETTIME
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
	rb_sys_fail("clock_gettime");
    sec = ts.tv_sec;
#else
    if (gettimeofday(&tv, NULL) == -1)
	rb_sys_fail("gettimeofday");
    sec = tv.tv_sec;
#endif
    tzset();
    if (!localtime_r(&sec, &tm))
	rb_sys_fail("localtime");

    y = tm.tm_year + 1900;
    m = tm.tm_mon + 1;
    d = tm.tm_mday;
    h = tm.tm_hour;
    min = tm.tm_min;
    s = tm.tm_sec;
    if (s == 60)
	s = 59;
#ifdef HAVE_STRUCT_TM_TM_GMTOFF
    of = tm.tm_gmtoff;
#elif defined(HAVE_TIMEZONE)
#if defined(HAVE_ALTZONE) && !defined(_AIX)
    of = (long)-((tm.tm_isdst > 0) ? altzone : timezone);
#else
    of = (long)-timezone;
    if (tm.tm_isdst) {
	time_t sec2;

	tm.tm_isdst = 0;
	sec2 = mktime(&tm);
	of += (long)difftime(sec2, sec);
    }
#endif
#elif defined(HAVE_TIMEGM)
    {
	time_t sec2;

	sec2 = timegm(&tm);
	of = (long)difftime(sec2, sec);
    }
#else
    {
	struct tm tm2;
	time_t sec2;

	if (!gmtime_r(&sec, &tm2))
	    rb_sys_fail("gmtime");
	tm2.tm_isdst = tm.tm_isdst;
	sec2 = mktime(&tm2);
	of = (long)difftime(sec, sec2);
    }
#endif
#ifdef HAVE_CLOCK_GETTIME
    sf = ts.tv_nsec;
#else
    sf = tv.tv_usec * 1000;
#endif

    if (of < -DAY_IN_SECONDS || of > DAY_IN_SECONDS) {
	of = 0;
	rb_warning("invalid offset is ignored");
    }

    decode_year(INT2FIX(y), -1, &nth, &ry);

    ret = d_complex_new_internal(klass,
				 nth, 0,
				 0, LONG2NUM(sf),
				 (int)of, GREGORIAN,
				 ry, m, d,
				 h, min, s,
				 HAVE_CIVIL | HAVE_TIME);
    {
	get_d1(ret);
	set_sg(dat, sg);
    }
    return ret;
}