void OnAcquireFileAccessPermissionDone(
    const content::ResourceRequestInfo::WebContentsGetter& web_contents_getter,
    const GURL& url,
    const std::string& request_method,
    CanDownloadCallback can_download_cb,
    bool granted) {
  if (granted) {
    CheckCanDownload(web_contents_getter, url, request_method,
                     std::move(can_download_cb));
  } else {
    std::move(can_download_cb).Run(false, false);
  }
}
