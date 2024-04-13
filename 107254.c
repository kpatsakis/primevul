rtadv_prefix_lookup (struct list *rplist, struct prefix_ipv6 *p)
{
  struct listnode *node;
  struct rtadv_prefix *rprefix;

  for (ALL_LIST_ELEMENTS_RO (rplist, node, rprefix))
    if (prefix_same ((struct prefix *) &rprefix->prefix, (struct prefix *) p))
      return rprefix;
  return NULL;
}
