uiserver_set_io_cbs (void *engine, gpgme_io_cbs_t io_cbs)
{
  engine_uiserver_t uiserver = engine;
  uiserver->io_cbs = *io_cbs;
}
