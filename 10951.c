tmx_m_zone(union DateData *x)
{
    VALUE zone = m_zone(x);
    /* TODO: fix potential dangling pointer */
    return RSTRING_PTR(zone);
}