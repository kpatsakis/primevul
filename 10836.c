date_s__strptime_internal(int argc, VALUE *argv, VALUE klass,
			  const char *default_fmt)
{
    VALUE vstr, vfmt, hash;
    const char *str, *fmt;
    size_t slen, flen;

    rb_scan_args(argc, argv, "11", &vstr, &vfmt);

    StringValue(vstr);
    if (!rb_enc_str_asciicompat_p(vstr))
	rb_raise(rb_eArgError,
		 "string should have ASCII compatible encoding");
    str = RSTRING_PTR(vstr);
    slen = RSTRING_LEN(vstr);
    if (argc < 2) {
	fmt = default_fmt;
	flen = strlen(default_fmt);
    }
    else {
	StringValue(vfmt);
	if (!rb_enc_str_asciicompat_p(vfmt))
	    rb_raise(rb_eArgError,
		     "format should have ASCII compatible encoding");
	fmt = RSTRING_PTR(vfmt);
	flen = RSTRING_LEN(vfmt);
    }
    hash = rb_hash_new();
    if (NIL_P(date__strptime(str, slen, fmt, flen, hash)))
	return Qnil;

    {
	VALUE zone = ref_hash("zone");
	VALUE left = ref_hash("leftover");

	if (!NIL_P(zone)) {
	    rb_enc_copy(zone, vstr);
	    set_hash("zone", zone);
	}
	if (!NIL_P(left)) {
	    rb_enc_copy(left, vstr);
	    set_hash("leftover", left);
	}
    }

    return hash;
}