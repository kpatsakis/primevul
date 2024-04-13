Eina_Bool ewk_view_disable_render(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api->disable_render, false);
    return smartData->api->disable_render(smartData);
}
