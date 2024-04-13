d_lite_httpdate(VALUE self)
{
    volatile VALUE dup = dup_obj_with_new_offset(self, 0);
    return strftimev("%a, %d %b %Y %T GMT", dup, set_tmx);
}