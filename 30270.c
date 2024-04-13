static bool name_is_device_color( char *cs_name )
{

    return( strcmp(cs_name, "DeviceGray") == 0 ||
            strcmp(cs_name, "DeviceRGB")  == 0 ||
            strcmp(cs_name, "DeviceCMYK") == 0);

}
