void ewk_view_title_set(Evas_Object* ewkView, const char* title)
{
    DBG("ewkView=%p, title=%s", ewkView, title ? title : "(null)");
    evas_object_smart_callback_call(ewkView, "title,changed", (void*)title);
}
