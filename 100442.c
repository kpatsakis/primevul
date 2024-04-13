float ewk_view_text_zoom_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, -1.0);
    return ewk_frame_text_zoom_get(smartData->main_frame);
}
