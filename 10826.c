d_lite_asctime(VALUE self)
{
    return strftimev("%a %b %e %H:%M:%S %Y", self, set_tmx);
}