DOMHighResTimeStamp PerformanceNavigationTiming::unloadEventEnd() const {
  bool allow_redirect_details = GetAllowRedirectDetails();
  DocumentLoadTiming* timing = GetDocumentLoadTiming();

  if (!allow_redirect_details || !timing ||
      !timing->HasSameOriginAsPreviousDocument())
    return 0;
  return Performance::MonotonicTimeToDOMHighResTimeStamp(
      TimeOrigin(), timing->UnloadEventEnd(), false /* allow_negative_value */);
}
