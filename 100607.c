Eina_Bool ewk_frame_contents_size_get(const Evas_Object* ewkFrame, Evas_Coord* width, Evas_Coord* height)
{
    if (width)
        *width = 0;
    if (height)
        *height = 0;
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    if (!smartData->frame || !smartData->frame->view())
        return false;
    if (width)
        *width = smartData->frame->view()->contentsWidth();
    if (height)
        *height = smartData->frame->view()->contentsHeight();
    return true;
}
