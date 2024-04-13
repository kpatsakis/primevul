get_limit(VALUE opt)
{
    if (!NIL_P(opt)) {
        VALUE limit = rb_hash_aref(opt, ID2SYM(rb_intern("limit")));
        if (NIL_P(limit)) return SIZE_MAX;
        return NUM2SIZET(limit);
    }
    return 128;
}