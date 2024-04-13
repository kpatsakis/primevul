Eina_Bool ewk_frame_feed_mouse_down(Evas_Object* ewkFrame, const Evas_Event_Mouse_Down* downEvent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(downEvent, false);

    WebCore::FrameView* view = smartData->frame->view();
    DBG("ewkFrame=%p, view=%p, button=%d, pos=%d,%d",
        ewkFrame, view, downEvent->button, downEvent->canvas.x, downEvent->canvas.y);
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, false);

    Evas_Coord x, y;
    evas_object_geometry_get(smartData->view, &x, &y, 0, 0);

    WebCore::PlatformMouseEvent event(downEvent, WebCore::IntPoint(x, y));
    return smartData->frame->eventHandler()->handleMousePressEvent(event);
}
