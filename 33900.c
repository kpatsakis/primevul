point_swap_cond (mpi_point_t d, mpi_point_t s, unsigned long swap,
                 mpi_ec_t ctx)
{
  mpi_swap_cond (d->x, s->x, swap);
  if (ctx->model != MPI_EC_MONTGOMERY)
    mpi_swap_cond (d->y, s->y, swap);
  mpi_swap_cond (d->z, s->z, swap);
}
