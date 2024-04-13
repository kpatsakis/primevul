partition_table_create_data_unref (CreatePartitionTableData *data)
{
  data->refcount--;
  if (data->refcount == 0)
    {
      g_object_unref (data->device);
      g_free (data->scheme);
      g_free (data);
    }
}
