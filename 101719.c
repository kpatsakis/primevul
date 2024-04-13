void ChromeDownloadManagerDelegate::OpenDownload(DownloadItem* download) {
  DCHECK_EQ(DownloadItem::COMPLETE, download->GetState());
  DCHECK(!download->GetTargetFilePath().empty());
  if (!download->CanOpenDownload())
    return;

  MaybeSendDangerousDownloadOpenedReport(download,
                                         false /* show_download_in_folder */);

  if (!DownloadItemModel(download).ShouldPreferOpeningInBrowser()) {
    RecordDownloadOpenMethod(DOWNLOAD_OPEN_METHOD_DEFAULT_PLATFORM);
    OpenDownloadUsingPlatformHandler(download);
    return;
  }

#if !defined(OS_ANDROID)
  content::WebContents* web_contents = download->GetWebContents();
  Browser* browser =
      web_contents ? chrome::FindBrowserWithWebContents(web_contents) : NULL;
  std::unique_ptr<chrome::ScopedTabbedBrowserDisplayer> browser_displayer;
  if (!browser ||
      !browser->CanSupportWindowFeature(Browser::FEATURE_TABSTRIP)) {
    browser_displayer.reset(new chrome::ScopedTabbedBrowserDisplayer(profile_));
    browser = browser_displayer->browser();
  }
  content::OpenURLParams params(
      net::FilePathToFileURL(download->GetTargetFilePath()),
      content::Referrer(), WindowOpenDisposition::NEW_FOREGROUND_TAB,
      ui::PAGE_TRANSITION_LINK, false);

  if (download->GetMimeType() == "application/x-x509-user-cert")
    chrome::ShowSettingsSubPage(browser, "certificates");
  else
    browser->OpenURL(params);

  RecordDownloadOpenMethod(DOWNLOAD_OPEN_METHOD_DEFAULT_BROWSER);
#else
  NOTREACHED();
 #endif
 }
