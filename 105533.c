linux_md_create_data_ref (LinuxMdCreateData *data)
{
  data->refcount++;
  return data;
}
