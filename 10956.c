d_lite_iso8601(VALUE self)
{
    return strftimev("%Y-%m-%d", self, set_tmx);
}