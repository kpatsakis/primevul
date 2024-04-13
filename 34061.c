pdf14_grayspot_get_color_comp_index(gx_device * dev, const char * pname,
    int name_size, int component_type)
{
    return pdf14_spot_get_color_comp_index(dev, pname, name_size, component_type, 1);
}
