pax_dump_header (struct tar_sparse_file *file)
{
  file->stat_info->sparse_major = tar_sparse_major;
  file->stat_info->sparse_minor = tar_sparse_minor;

  return (file->stat_info->sparse_major == 0) ?
           pax_dump_header_0 (file) : pax_dump_header_1 (file);
}
