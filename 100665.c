void WebContentsImpl::DidStopLoading(RenderViewHost* render_view_host) {
  scoped_ptr<LoadNotificationDetails> details;

  NavigationEntry* entry = controller_.GetLastCommittedEntry();

  if (entry) {
    base::TimeDelta elapsed = base::TimeTicks::Now() - current_load_start_;

    details.reset(new LoadNotificationDetails(
        entry->GetVirtualURL(),
        entry->GetTransitionType(),
        elapsed,
        &controller_,
        controller_.GetCurrentEntryIndex()));
  }

  SetIsLoading(false, details.get());

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidStopLoading(render_view_host));
}
