eXosip_set_cbsip_message (struct eXosip_t *excontext, CbSipCallback cbsipCallback)
{
  excontext->cbsipCallback = cbsipCallback;
  return 0;
}
