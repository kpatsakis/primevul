ec_mod (gcry_mpi_t w, mpi_ec_t ec)
{
  if (0 && ec->dialect == ECC_DIALECT_ED25519)
    _gcry_mpi_ec_ed25519_mod (w);
  else if (ec->t.p_barrett)
    _gcry_mpi_mod_barrett (w, w, ec->t.p_barrett);
  else
    _gcry_mpi_mod (w, w, ec->p);
}
