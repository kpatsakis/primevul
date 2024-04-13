jisx0301_date_format(char *fmt, size_t size, VALUE jd, VALUE y)
{
    if (FIXNUM_P(jd)) {
	long d = FIX2INT(jd);
	long s;
	char c;
	if (d < 2405160)
	    return "%Y-%m-%d";
	if (d < 2419614) {
	    c = 'M';
	    s = 1867;
	}
	else if (d < 2424875) {
	    c = 'T';
	    s = 1911;
	}
	else if (d < 2447535) {
	    c = 'S';
	    s = 1925;
	}
	else if (d < 2458605) {
	    c = 'H';
	    s = 1988;
	}
	else {
	    c = 'R';
	    s = 2018;
	}
	snprintf(fmt, size, "%c%02ld" ".%%m.%%d", c, FIX2INT(y) - s);
	return fmt;
    }
    return "%Y-%m-%d";
}