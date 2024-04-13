TopSitesImpl::TopSitesImpl(PrefService* pref_service,
                           HistoryService* history_service,
                           std::unique_ptr<TopSitesProvider> provider,
                           const PrepopulatedPageList& prepopulated_pages,
                           const CanAddURLToHistoryFn& can_add_url_to_history)
    : backend_(nullptr),
      cache_(base::MakeUnique<TopSitesCache>()),
      thread_safe_cache_(base::MakeUnique<TopSitesCache>()),
      prepopulated_pages_(prepopulated_pages),
      pref_service_(pref_service),
      history_service_(history_service),
      provider_(std::move(provider)),
      can_add_url_to_history_(can_add_url_to_history),
      loaded_(false),
      history_service_observer_(this) {
  DCHECK(pref_service_);
  DCHECK(!can_add_url_to_history_.is_null());
  DCHECK(provider_);
}
