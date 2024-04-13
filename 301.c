bool SortByHost(const GURL& lhs, const GURL& rhs) {
  return net::GetHostOrSpecFromURL(lhs) > net::GetHostOrSpecFromURL(rhs);
}
