date_strftime_internal(int argc, VALUE *argv, VALUE self,
		       const char *default_fmt,
		       void (*func)(VALUE, struct tmx *))
{
    VALUE vfmt;
    const char *fmt;
    long len;
    char buffer[SMALLBUF], *buf = buffer;
    struct tmx tmx;
    VALUE str;

    rb_scan_args(argc, argv, "01", &vfmt);

    if (argc < 1)
	vfmt = rb_usascii_str_new2(default_fmt);
    else {
	StringValue(vfmt);
	if (!rb_enc_str_asciicompat_p(vfmt)) {
	    rb_raise(rb_eArgError,
		     "format should have ASCII compatible encoding");
	}
    }
    fmt = RSTRING_PTR(vfmt);
    len = RSTRING_LEN(vfmt);
    (*func)(self, &tmx);
    if (memchr(fmt, '\0', len)) {
	/* Ruby string may contain \0's. */
	const char *p = fmt, *pe = fmt + len;

	str = rb_str_new(0, 0);
	while (p < pe) {
	    len = date_strftime_alloc(&buf, p, &tmx);
	    rb_str_cat(str, buf, len);
	    p += strlen(p);
	    if (buf != buffer) {
		xfree(buf);
		buf = buffer;
	    }
	    for (fmt = p; p < pe && !*p; ++p);
	    if (p > fmt) rb_str_cat(str, fmt, p - fmt);
	}
	rb_enc_copy(str, vfmt);
	return str;
    }
    else
	len = date_strftime_alloc(&buf, fmt, &tmx);

    str = rb_str_new(buf, len);
    if (buf != buffer) xfree(buf);
    rb_enc_copy(str, vfmt);
    return str;
}