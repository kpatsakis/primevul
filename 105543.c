linux_md_start_data_ref (LinuxMdStartData *data)
{
  data->refcount++;
  return data;
}
