pax_start_header (struct tar_stat_info *st)
{
  off_t realsize = st->stat.st_size;
  union block *blk;
  st->stat.st_size = st->archive_file_size;
  blk = start_header (st);
  st->stat.st_size = realsize;
  return blk;
}
