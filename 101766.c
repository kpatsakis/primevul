bool TopSitesImpl::AddPrepopulatedPages(MostVisitedURLList* urls,
                                        size_t num_forced_urls) const {
  bool added = false;
  for (const auto& prepopulated_page : prepopulated_pages_) {
    if (urls->size() - num_forced_urls < kNonForcedTopSitesNumber &&
        IndexOf(*urls, prepopulated_page.most_visited.url) == -1) {
      urls->push_back(prepopulated_page.most_visited);
      added = true;
    }
  }
  return added;
}
