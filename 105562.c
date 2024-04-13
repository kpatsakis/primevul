lvm2_lv_create_data_ref (CreateLvm2LVData *data)
{
  data->refcount++;
  return data;
}
