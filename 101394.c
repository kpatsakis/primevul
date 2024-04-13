png_get_sCAL_s(png_structp png_ptr, png_infop info_ptr,
             int *unit, png_charpp width, png_charpp height)
{
    if (png_ptr != NULL && info_ptr != NULL &&
        (info_ptr->valid & PNG_INFO_sCAL))
    {
        *unit = info_ptr->scal_unit;
        *width = info_ptr->scal_s_width;
        *height = info_ptr->scal_s_height;
        return (PNG_INFO_sCAL);
    }
    return(0);
}
