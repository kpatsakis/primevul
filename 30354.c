no_proxy_match (const char *host, const char **no_proxy)
{
  if (!no_proxy)
    return false;
  else
    return sufmatch (no_proxy, host);
}
