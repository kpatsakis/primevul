ec_pow3 (gcry_mpi_t w, const gcry_mpi_t b, mpi_ec_t ctx)
{
  mpi_powm (w, b, mpi_const (MPI_C_THREE), ctx->p);
}
