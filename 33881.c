_gcry_mpi_point_init (mpi_point_t p)
{
  p->x = mpi_new (0);
  p->y = mpi_new (0);
  p->z = mpi_new (0);
}
