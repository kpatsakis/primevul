_gcry_mpi_point_snatch_get (gcry_mpi_t x, gcry_mpi_t y, gcry_mpi_t z,
                            mpi_point_t point)
{
  mpi_snatch (x, point->x);
  mpi_snatch (y, point->y);
  mpi_snatch (z, point->z);
  xfree (point);
}
