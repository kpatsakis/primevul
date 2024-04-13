strftimev(const char *fmt, VALUE self,
	  void (*func)(VALUE, struct tmx *))
{
    char buffer[SMALLBUF], *buf = buffer;
    struct tmx tmx;
    long len;
    VALUE str;

    (*func)(self, &tmx);
    len = date_strftime_alloc(&buf, fmt, &tmx);
    RB_GC_GUARD(self);
    str = rb_usascii_str_new(buf, len);
    if (buf != buffer) xfree(buf);
    return str;
}