check_numeric(VALUE obj, const char* field) {
    if(!RTEST(rb_obj_is_kind_of(obj, rb_cNumeric))) {
        rb_raise(rb_eTypeError, "invalid %s (not numeric)", field);
    }
}