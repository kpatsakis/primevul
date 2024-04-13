Eina_Bool ewk_frame_feed_key_down(Evas_Object* ewkFrame, const Evas_Event_Key_Down* downEvent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(downEvent, false);

    DBG("ewkFrame=%p keyname=%s (key=%s, string=%s)",
        ewkFrame, downEvent->keyname, downEvent->key ? downEvent->key : "", downEvent->string ? downEvent->string : "");

    WebCore::PlatformKeyboardEvent event(downEvent);
    if (smartData->frame->eventHandler()->keyEvent(event))
        return true;

    return _ewk_frame_handle_key_scrolling(smartData->frame, event);
}
