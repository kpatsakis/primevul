ec_addm (gcry_mpi_t w, gcry_mpi_t u, gcry_mpi_t v, mpi_ec_t ctx)
{
  mpi_add (w, u, v);
  ec_mod (w, ctx);
}
