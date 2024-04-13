Eina_Bool ewk_view_font_family_name_set(Evas_Object* ewkView, Ewk_Font_Family fontFamily, const char* name)
{
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    switch (fontFamily) {
    case EWK_FONT_FAMILY_STANDARD:
        if (eina_stringshare_replace(&priv->settings.fontStandard, name))
            priv->pageSettings->setStandardFontFamily(AtomicString::fromUTF8(name));
        break;
    case EWK_FONT_FAMILY_CURSIVE:
        if (eina_stringshare_replace(&priv->settings.fontCursive, name))
            priv->pageSettings->setCursiveFontFamily(AtomicString::fromUTF8(name));
        break;
    case EWK_FONT_FAMILY_FANTASY:
        if (eina_stringshare_replace(&priv->settings.fontFantasy, name))
            priv->pageSettings->setFantasyFontFamily(AtomicString::fromUTF8(name));
        break;
    case EWK_FONT_FAMILY_MONOSPACE:
        if (eina_stringshare_replace(&priv->settings.fontMonospace, name))
            priv->pageSettings->setFixedFontFamily(AtomicString::fromUTF8(name));
        break;
    case EWK_FONT_FAMILY_SERIF:
        if (eina_stringshare_replace(&priv->settings.fontSerif, name))
            priv->pageSettings->setSerifFontFamily(AtomicString::fromUTF8(name));
        break;
    case EWK_FONT_FAMILY_SANS_SERIF:
        if (eina_stringshare_replace(&priv->settings.fontSansSerif, name))
            priv->pageSettings->setSansSerifFontFamily(AtomicString::fromUTF8(name));
        break;
    default:
        return false;
    }

    return true;
}
