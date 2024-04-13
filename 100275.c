void RenderView::didFinishResourceLoad(
    WebFrame* frame, unsigned identifier) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->dataSource());
  if (!navigation_state->postpone_loading_data())
    return;


  const GURL& frame_url = frame->url();

  const GURL& error_page_url = GetAlternateErrorPageURL(frame_url, HTTP_404);
  DCHECK(error_page_url.is_valid());

  WebURLError original_error;
  original_error.unreachableURL = frame_url;

  navigation_state->set_alt_error_page_fetcher(
      new AltErrorPageResourceFetcher(
          error_page_url, frame, original_error,
          NewCallback(this, &RenderView::AltErrorPageFinished)));
}
