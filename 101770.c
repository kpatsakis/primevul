void TopSitesImpl::OnNavigationCommitted(const GURL& url) {
  DCHECK(thread_checker_.CalledOnValidThread());
  if (!loaded_)
    return;

  if (can_add_url_to_history_.Run(url))
    ScheduleUpdateTimer();
}
