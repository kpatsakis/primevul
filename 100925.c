void RenderViewImpl::NavigateToSwappedOutURL(WebKit::WebFrame* frame) {
  CHECK(is_swapped_out_);
  GURL swappedOutURL(kSwappedOutURL);
  WebURLRequest request(swappedOutURL);
  frame->loadRequest(request);
}
