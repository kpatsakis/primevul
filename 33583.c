gpgsm_io_event (void *engine, gpgme_event_io_t type, void *type_data)
{
  engine_gpgsm_t gpgsm = engine;

  TRACE3 (DEBUG_ENGINE, "gpgme:gpgsm_io_event", gpgsm,
          "event %p, type %d, type_data %p",
          gpgsm->io_cbs.event, type, type_data);
  if (gpgsm->io_cbs.event)
    (*gpgsm->io_cbs.event) (gpgsm->io_cbs.event_priv, type, type_data);
}
