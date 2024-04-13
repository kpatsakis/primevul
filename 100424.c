void ewk_view_add_console_message(Evas_Object* ewkView, const char* message, unsigned int lineNumber, const char* sourceID)
{
    DBG("ewkView=%p message=%s lineNumber=%u sourceID=%s", ewkView, message, lineNumber, sourceID);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api);
    EINA_SAFETY_ON_NULL_RETURN(smartData->api->add_console_message);
    smartData->api->add_console_message(smartData, message, lineNumber, sourceID);
}
