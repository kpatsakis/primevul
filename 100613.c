void ewk_frame_mixed_content_run_set(Evas_Object* ewkFrame, bool hasRun)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData);
    smartData->hasRunMixedContent = hasRun;

    if (hasRun) {
        ewk_view_mixed_content_run_set(smartData->view, true);
        evas_object_smart_callback_call(ewkFrame, "mixedcontent,run", 0);
    }
}
