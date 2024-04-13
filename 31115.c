oldgnu_add_sparse (struct tar_sparse_file *file, struct sparse *s)
{
  struct sp_array sp;

  if (s->numbytes[0] == '\0')
    return add_finish;
  sp.offset = OFF_FROM_HEADER (s->offset);
  sp.numbytes = OFF_FROM_HEADER (s->numbytes);
  if (sp.offset < 0 || sp.numbytes < 0
      || INT_ADD_OVERFLOW (sp.offset, sp.numbytes)
      || file->stat_info->stat.st_size < sp.offset + sp.numbytes
      || file->stat_info->archive_file_size < 0)
    return add_fail;

  sparse_add_map (file->stat_info, &sp);
  return add_ok;
}
