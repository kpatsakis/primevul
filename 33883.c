_gcry_mpi_point_new (unsigned int nbits)
{
  mpi_point_t p;

  (void)nbits;  /* Currently not used.  */

  p = xmalloc (sizeof *p);
  _gcry_mpi_point_init (p);
  return p;
}
