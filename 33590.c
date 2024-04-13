gpgsm_set_io_cbs (void *engine, gpgme_io_cbs_t io_cbs)
{
  engine_gpgsm_t gpgsm = engine;
  gpgsm->io_cbs = *io_cbs;
}
