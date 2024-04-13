Eina_Bool ewk_frame_feed_mouse_move(Evas_Object* ewkFrame, const Evas_Event_Mouse_Move* moveEvent)
{
    EWK_FRAME_SD_GET_OR_RETURN(ewkFrame, smartData, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(smartData->frame, false);
    EINA_SAFETY_ON_NULL_RETURN_VAL(moveEvent, false);

    WebCore::FrameView* view = smartData->frame->view();
    DBG("ewkFrame=%p, view=%p, pos: old=%d,%d, new=%d,%d, buttons=%d",
        ewkFrame, view, moveEvent->cur.canvas.x, moveEvent->cur.canvas.y,
        moveEvent->prev.canvas.x, moveEvent->prev.canvas.y, moveEvent->buttons);
    EINA_SAFETY_ON_NULL_RETURN_VAL(view, false);

    Evas_Coord x, y;
    evas_object_geometry_get(smartData->view, &x, &y, 0, 0);

    WebCore::PlatformMouseEvent event(moveEvent, WebCore::IntPoint(x, y));
    return smartData->frame->eventHandler()->mouseMoved(event);
}
