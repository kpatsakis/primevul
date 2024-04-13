uint64_t ewk_view_exceeded_database_quota(Evas_Object* ewkView, Evas_Object* frame, const char* databaseName, uint64_t currentSize, uint64_t expectedSize)
{
    DBG("ewkView=%p", ewkView);
    EWK_VIEW_SD_GET_OR_RETURN(ewkView, smartData, 0);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->api, 0);
    if (!smartData->api->exceeded_database_quota)
        return 0;

    INF("currentSize=%" PRIu64 " expectedSize=%" PRIu64, currentSize, expectedSize);
    return smartData->api->exceeded_database_quota(smartData, frame, databaseName, currentSize, expectedSize);
}
