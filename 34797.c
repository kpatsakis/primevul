eXosip_guess_localip (struct eXosip_t *excontext, int family, char *address, int size)
{
  return _eXosip_guess_ip_for_via (excontext, family, address, size);
}
