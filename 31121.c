pax_dump_header_0 (struct tar_sparse_file *file)
{
  off_t block_ordinal = current_block_ordinal ();
  union block *blk;
  size_t i;
  char nbuf[UINTMAX_STRSIZE_BOUND];
  struct sp_array *map = file->stat_info->sparse_map;
  char *save_file_name = NULL;

  /* Store the real file size */
  xheader_store ("GNU.sparse.size", file->stat_info, NULL);
  xheader_store ("GNU.sparse.numblocks", file->stat_info, NULL);

  if (xheader_keyword_deleted_p ("GNU.sparse.map")
      || tar_sparse_minor == 0)
    {
      for (i = 0; i < file->stat_info->sparse_map_avail; i++)
	{
	  xheader_store ("GNU.sparse.offset", file->stat_info, &i);
	  xheader_store ("GNU.sparse.numbytes", file->stat_info, &i);
	}
    }
  else
    {
      xheader_store ("GNU.sparse.name", file->stat_info, NULL);
      save_file_name = file->stat_info->file_name;
      file->stat_info->file_name = xheader_format_name (file->stat_info,
					       "%d/GNUSparseFile.%p/%f", 0);

      xheader_string_begin (&file->stat_info->xhdr);
      for (i = 0; i < file->stat_info->sparse_map_avail; i++)
	{
	  if (i)
	    xheader_string_add (&file->stat_info->xhdr, ",");
	  xheader_string_add (&file->stat_info->xhdr,
			      umaxtostr (map[i].offset, nbuf));
	  xheader_string_add (&file->stat_info->xhdr, ",");
	  xheader_string_add (&file->stat_info->xhdr,
			      umaxtostr (map[i].numbytes, nbuf));
	}
      if (!xheader_string_end (&file->stat_info->xhdr,
			       "GNU.sparse.map"))
	{
	  free (file->stat_info->file_name);
	  file->stat_info->file_name = save_file_name;
	  return false;
	}
    }
  blk = pax_start_header (file->stat_info);
  finish_header (file->stat_info, blk, block_ordinal);
  if (save_file_name)
    {
      free (file->stat_info->file_name);
      file->stat_info->file_name = save_file_name;
    }
  return true;
}
