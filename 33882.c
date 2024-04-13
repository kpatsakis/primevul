_gcry_mpi_point_log (const char *name, mpi_point_t point, mpi_ec_t ctx)
{
  gcry_mpi_t x, y;
  char buf[100];

  if (!point)
    {
      snprintf (buf, sizeof buf - 1, "%s.*", name);
      log_mpidump (buf, NULL);
      return;
    }
  snprintf (buf, sizeof buf - 1, "%s.X", name);

  if (ctx)
    {
      x = mpi_new (0);
      y = mpi_new (0);
    }
  if (!ctx || _gcry_mpi_ec_get_affine (x, y, point, ctx))
    {
      log_mpidump (buf, point->x);
      buf[strlen(buf)-1] = 'Y';
      log_mpidump (buf, point->y);
      buf[strlen(buf)-1] = 'Z';
      log_mpidump (buf, point->z);
    }
  else
    {
      buf[strlen(buf)-1] = 'x';
      log_mpidump (buf, x);
      buf[strlen(buf)-1] = 'y';
      log_mpidump (buf, y);

    }
  if (ctx)
    {
      _gcry_mpi_release (x);
      _gcry_mpi_release (y);
    }
}
