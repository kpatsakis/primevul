void RenderView::AddSearchProvider(const std::string& url) {
  AddGURLSearchProvider(GURL(url),
                        false);  // not autodetected
}
