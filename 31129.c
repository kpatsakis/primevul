star_sparse_member_p (struct tar_sparse_file *file __attribute__ ((unused)))
{
  return current_header->header.typeflag == GNUTYPE_SPARSE;
}
