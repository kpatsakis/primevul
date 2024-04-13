pax_sparse_member_p (struct tar_sparse_file *file)
{
  return file->stat_info->sparse_map_avail > 0
          || file->stat_info->sparse_major > 0;
}
