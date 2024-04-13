d_lite_inspect(VALUE self)
{
    get_d1(self);
    return mk_inspect(dat, rb_obj_class(self), self);
}