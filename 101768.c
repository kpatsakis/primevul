bool TopSitesImpl::HasBlacklistedItems() const {
  const base::DictionaryValue* blacklist =
      pref_service_->GetDictionary(kMostVisitedURLsBlacklist);
  return blacklist && !blacklist->empty();
}
