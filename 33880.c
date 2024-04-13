_gcry_mpi_point_get (gcry_mpi_t x, gcry_mpi_t y, gcry_mpi_t z,
                     mpi_point_t point)
{
  if (x)
    mpi_set (x, point->x);
  if (y)
    mpi_set (y, point->y);
  if (z)
    mpi_set (z, point->z);
}
