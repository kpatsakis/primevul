void ewk_view_load_document_finished(Evas_Object* ewkView, Evas_Object* frame)
{
    evas_object_smart_callback_call(ewkView, "load,document,finished", frame);
}
