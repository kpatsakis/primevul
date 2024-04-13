bool TopSitesCache::IsKnownURL(const GURL& url) const {
  return GetCanonicalURLsIterator(url) != canonical_urls_.end();
}
