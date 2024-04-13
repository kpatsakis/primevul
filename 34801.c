eXosip_reset_transports (struct eXosip_t *excontext)
{
  int i = OSIP_WRONG_STATE;

  if (excontext->eXtl_transport.tl_reset)
    i = excontext->eXtl_transport.tl_reset (excontext);
  return i;
}
