void Browser::ViewSourceForTab(WebContents* source, const GURL& page_url) {
  DCHECK(source);
  chrome::ViewSource(this, source);
}
