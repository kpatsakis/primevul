uiserver_release (void *engine)
{
  engine_uiserver_t uiserver = engine;

  if (!uiserver)
    return;

  uiserver_cancel (engine);

  free (uiserver->colon.attic.line);
  free (uiserver);
}
