ec_mul2 (gcry_mpi_t w, gcry_mpi_t u, mpi_ec_t ctx)
{
  mpi_lshift (w, u, 1);
  ec_mod (w, ctx);
}
