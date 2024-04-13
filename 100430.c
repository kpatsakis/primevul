void ewk_view_js_window_object_clear(Evas_Object* ewkView, Evas_Object* frame)
{
    evas_object_smart_callback_call(ewkView, "js,windowobject,clear", frame);
}
