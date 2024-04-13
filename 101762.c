    TopSitesCache::GetCanonicalURLsIterator(const GURL& url) const {
  return canonical_urls_.find(CanonicalURLQuery(url).entry());
}
