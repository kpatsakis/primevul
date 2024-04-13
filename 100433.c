Eina_Bool ewk_view_navigate(Evas_Object* ewkView, int steps)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    return ewk_frame_navigate(smartData->main_frame, steps);
}
