sparse_dump_region (struct tar_sparse_file *file, size_t i)
{
  union block *blk;
  off_t bytes_left = file->stat_info->sparse_map[i].numbytes;

  if (!lseek_or_error (file, file->stat_info->sparse_map[i].offset))
    return false;

  while (bytes_left > 0)
    {
      size_t bufsize = (bytes_left > BLOCKSIZE) ? BLOCKSIZE : bytes_left;
      size_t bytes_read;

      blk = find_next_block ();
      bytes_read = safe_read (file->fd, blk->buffer, bufsize);
      if (bytes_read == SAFE_READ_ERROR)
	{
          read_diag_details (file->stat_info->orig_file_name,
	                     (file->stat_info->sparse_map[i].offset
			      + file->stat_info->sparse_map[i].numbytes
			      - bytes_left),
			     bufsize);
	  return false;
	}
      else if (bytes_read == 0)
	{
	  char buf[UINTMAX_STRSIZE_BOUND];
	  struct stat st;
	  size_t n;
	  if (fstat (file->fd, &st) == 0)
	    n = file->stat_info->stat.st_size - st.st_size;
	  else
	    n = file->stat_info->stat.st_size
	      - (file->stat_info->sparse_map[i].offset
		 + file->stat_info->sparse_map[i].numbytes
		 - bytes_left);
	  
	  WARNOPT (WARN_FILE_SHRANK,
		   (0, 0,
		    ngettext ("%s: File shrank by %s byte; padding with zeros",
			      "%s: File shrank by %s bytes; padding with zeros",
			      n),
		    quotearg_colon (file->stat_info->orig_file_name),
		    STRINGIFY_BIGINT (n, buf)));
	  if (! ignore_failed_read_option)
	    set_exit_status (TAREXIT_DIFFERS);
	  return false;
	}

      memset (blk->buffer + bytes_read, 0, BLOCKSIZE - bytes_read);
      bytes_left -= bytes_read;
      file->dumped_size += bytes_read;
      set_next_block_after (blk);
    }

  return true;
}
