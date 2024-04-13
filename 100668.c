WebContents* WebContentsImpl::GetEmbedderWebContents() const {
  BrowserPluginGuest* guest = GetBrowserPluginGuest();
  if (guest)
    return guest->embedder_web_contents();
  return NULL;
}
