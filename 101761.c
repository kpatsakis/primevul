TopSitesCache::CanonicalURLQuery::CanonicalURLQuery(const GURL& url) {
  most_visited_url_.redirects.push_back(url);
  entry_.first = &most_visited_url_;
  entry_.second = 0u;
}
