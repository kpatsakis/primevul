date_s__load(VALUE klass, VALUE s)
{
    VALUE a, obj;

    a = rb_marshal_load(s);
    obj = d_lite_s_alloc(klass);
    return d_lite_marshal_load(obj, a);
}