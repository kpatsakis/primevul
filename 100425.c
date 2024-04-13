float ewk_view_device_pixel_ratio_get(const Evas_Object* ewkView)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, -1.0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, -1.0);

    return priv->settings.devicePixelRatio;
}
