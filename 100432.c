void ewk_view_load_started(Evas_Object* ewkView)
{
    DBG("ewkView=%p", ewkView);
    evas_object_smart_callback_call(ewkView, "load,started", 0);
}
