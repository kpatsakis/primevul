ec_pow2 (gcry_mpi_t w, const gcry_mpi_t b, mpi_ec_t ctx)
{
  /* Using mpi_mul is slightly faster (at least on amd64).  */
  /* mpi_powm (w, b, mpi_const (MPI_C_TWO), ctx->p); */
  ec_mulm (w, b, b, ctx);
}
