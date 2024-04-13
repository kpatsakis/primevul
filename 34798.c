eXosip_malloc (void)
{
  struct eXosip_t *ptr = (struct eXosip_t *) osip_malloc (sizeof (eXosip_t));

  if (ptr)
    memset (ptr, 0, sizeof (eXosip_t));
  return ptr;
}
