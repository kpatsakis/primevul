void ewk_view_toolbars_visible_get(Evas_Object* ewkView, bool* visible)
{
    DBG("%s, o=%p", __func__, ewkView);
    *visible = false;
    evas_object_smart_callback_call(ewkView, "toolbars,visible,get", visible);
}
