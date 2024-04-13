iso8601_timediv(VALUE self, long n)
{
    static const char timefmt[] = "T%H:%M:%S";
    static const char zone[] = "%:z";
    char fmt[sizeof(timefmt) + sizeof(zone) + rb_strlen_lit(".%N") +
	     DECIMAL_SIZE_OF_LONG];
    char *p = fmt;

    memcpy(p, timefmt, sizeof(timefmt)-1);
    p += sizeof(timefmt)-1;
    if (n > 0) p += snprintf(p, fmt+sizeof(fmt)-p, ".%%%ldN", n);
    memcpy(p, zone, sizeof(zone));
    return strftimev(fmt, self, set_tmx);
}