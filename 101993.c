std::unique_ptr<NavigationEntry> NavigationController::CreateNavigationEntry(
    const GURL& url,
    const Referrer& referrer,
    ui::PageTransition transition,
    bool is_renderer_initiated,
    const std::string& extra_headers,
    BrowserContext* browser_context,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory) {
  GURL dest_url(url);
  BrowserURLHandlerImpl::GetInstance()->FixupURLBeforeRewrite(&dest_url,
                                                              browser_context);

  GURL loaded_url(dest_url);
  bool reverse_on_redirect = false;
  BrowserURLHandlerImpl::GetInstance()->RewriteURLIfNecessary(
      &loaded_url, browser_context, &reverse_on_redirect);

  NavigationEntryImpl* entry = new NavigationEntryImpl(
      nullptr,  // The site instance for tabs is sent on navigation
      loaded_url, referrer, base::string16(), transition, is_renderer_initiated,
      blob_url_loader_factory);
  entry->SetVirtualURL(dest_url);
  entry->set_user_typed_url(dest_url);
  entry->set_update_virtual_url_with_url(reverse_on_redirect);
  entry->set_extra_headers(extra_headers);
  return base::WrapUnique(entry);
}
