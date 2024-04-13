uiserver_io_event (void *engine, gpgme_event_io_t type, void *type_data)
{
  engine_uiserver_t uiserver = engine;

  TRACE3 (DEBUG_ENGINE, "gpgme:uiserver_io_event", uiserver,
          "event %p, type %d, type_data %p",
          uiserver->io_cbs.event, type, type_data);
  if (uiserver->io_cbs.event)
    (*uiserver->io_cbs.event) (uiserver->io_cbs.event_priv, type, type_data);
}
