dt_lite_to_s(VALUE self)
{
    return strftimev("%Y-%m-%dT%H:%M:%S%:z", self, set_tmx);
}