static Eina_Bool _ewk_view_smart_pre_render_region(Ewk_View_Smart_Data* smartData, Evas_Coord x, Evas_Coord y, Evas_Coord width, Evas_Coord height, float zoom)
{
    WRN("not supported by engine. smartDAta=%p area=%d,%d+%dx%d, zoom=%f",
        smartData, x, y, width, height, zoom);
    return false;
}
