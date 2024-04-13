sparse_diff_file (int fd, struct tar_stat_info *st)
{
  bool rc = true;
  struct tar_sparse_file file;
  size_t i;
  off_t offset = 0;

  if (!tar_sparse_init (&file))
    return dump_status_not_implemented;

  file.stat_info = st;
  file.fd = fd;
  file.seekable = true; /* File *must* be seekable for compare to work */

  rc = tar_sparse_decode_header (&file);
  mv_begin_read (st);
  for (i = 0; rc && i < file.stat_info->sparse_map_avail; i++)
    {
      rc = check_sparse_region (&file,
				offset, file.stat_info->sparse_map[i].offset)
	    && check_data_region (&file, i);
      offset = file.stat_info->sparse_map[i].offset
	        + file.stat_info->sparse_map[i].numbytes;
    }

  if (!rc)
    skip_file (file.stat_info->archive_file_size - file.dumped_size);
  mv_end ();

  tar_sparse_done (&file);
  return rc;
}
