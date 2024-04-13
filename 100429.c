WTF::PassRefPtr<WebCore::Frame> ewk_view_frame_create(Evas_Object* ewkView, Evas_Object* frame, const WTF::String& name, WebCore::HTMLFrameOwnerElement* ownerElement, const WebCore::KURL& url, const WTF::String& referrer)
{
    DBG("ewkView=%p, frame=%p, name=%s, ownerElement=%p, url=%s, referrer=%s",
        ewkView, frame, name.utf8().data(), ownerElement,
        url.string().utf8().data(), referrer.utf8().data());

    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EWK_VIEW_PRIV_GET_OR_RETURN(smartData, priv, 0);

    WTF::RefPtr<WebCore::Frame> coreFrame = _ewk_view_core_frame_new
                                         (smartData, priv, ownerElement);
    if (!coreFrame) {
        ERR("Could not create child core frame '%s'", name.utf8().data());
        return 0;
    }

    if (!ewk_frame_child_add(frame, coreFrame, name, url, referrer)) {
        ERR("Could not create child frame object '%s'", name.utf8().data());
        return 0;
    }

    if (!coreFrame->page() || !coreFrame->tree() || !coreFrame->tree()->parent())
        return 0;

    smartData->changed.frame_rect = true;
    _ewk_view_smart_changed(smartData);

    return coreFrame.release();
}
