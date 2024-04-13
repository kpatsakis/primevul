Eina_Bool ewk_view_popup_destroy(Evas_Object* ewkView)
{
    INF("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, false);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, false);

    if (!priv->popup.menuClient)
        return false;

    evas_object_smart_callback_call(ewkView, "popup,willdelete", &priv->popup.menu);

    void* itemv;
    EINA_LIST_FREE(priv->popup.menu.items, itemv) {
        Ewk_Menu_Item* item = static_cast<Ewk_Menu_Item*>(itemv);
        eina_stringshare_del(item->text);
        free(item);
    }
    priv->popup.menuClient->popupDidHide();
    priv->popup.menuClient = 0;

    return true;
}
