update_info_in_idle_device_unreffed (gpointer user_data,
                                     GObject *where_the_object_was)
{
  UpdateInfoInIdleData *data = user_data;
  g_source_remove (data->idle_id);
}
