bool TopSitesImpl::IsForcedFull() {
  return loaded_ && cache_->GetNumForcedURLs() >= kForcedTopSitesNumber;
}
