Eina_Bool ewk_view_setting_encoding_default_set(Evas_Object* ewkView, const char* encoding)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);
    if (eina_stringshare_replace(&priv->settings.encodingDefault, encoding))
        priv->pageSettings->setDefaultTextEncodingName(String::fromUTF8(encoding));
    return true;
}
