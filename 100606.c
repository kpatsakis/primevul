Evas_Object* ewk_frame_child_find(Evas_Object* ewkFrame, const char* name)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(name, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, 0);
    WTF::String frameName = WTF::String::fromUTF8(name);
    return EWKPrivate::kitFrame(smartData->frame->tree()->find(WTF::AtomicString(frameName)));
}
