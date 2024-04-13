void ewk_view_statusbar_text_set(Evas_Object* ewkView, const char* text)
{
    DBG("ewkView=%p (text=%s)", ewkView, text);
    INF("status bar text set: %s", text);
    evas_object_smart_callback_call(ewkView, "statusbar,text,set", (void*)text);
}
