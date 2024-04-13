uiserver_reset (void *engine)
{
  engine_uiserver_t uiserver = engine;

  /* We must send a reset because we need to reset the list of
     signers.  Note that RESET does not reset OPTION commands. */
  return uiserver_assuan_simple_command (uiserver->assuan_ctx, "RESET", NULL, NULL);
}
