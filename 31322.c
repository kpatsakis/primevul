g_proxy_list_free (DBusGProxyList *list)
{
  /* we don't hold a reference to the proxies in the list,
   * as they ref the GProxyManager
   */
  g_slist_free (list->proxies);  

  g_free (list);
}
