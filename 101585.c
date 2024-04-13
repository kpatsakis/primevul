HistoryController::HistoryController(RenderViewImpl* render_view)
    : render_view_(render_view) {
  DCHECK(!SiteIsolationPolicy::UseSubframeNavigationEntries());
}
