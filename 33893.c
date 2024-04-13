ec_mulm (gcry_mpi_t w, gcry_mpi_t u, gcry_mpi_t v, mpi_ec_t ctx)
{
  mpi_mul (w, u, v);
  ec_mod (w, ctx);
}
