partition_modify_data_unref (ModifyPartitionData *data)
{
  g_object_unref (data->device);
  g_object_unref (data->enclosing_device);
  g_free (data->type);
  g_free (data->label);
  g_free (data->flags);
  g_free (data);
}
