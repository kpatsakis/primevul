bool ChromeDownloadManagerDelegate::DetermineDownloadTarget(
    DownloadItem* download,
    const content::DownloadTargetCallback& callback) {
  if (download->GetTargetFilePath().empty() &&
      download->GetMimeType() == kPDFMimeType && !download->HasUserGesture()) {
    ReportPDFLoadStatus(PDFLoadStatus::kTriggeredNoGestureDriveByDownload);
  }

  DownloadTargetDeterminer::CompletionCallback target_determined_callback =
      base::Bind(&ChromeDownloadManagerDelegate::OnDownloadTargetDetermined,
                 weak_ptr_factory_.GetWeakPtr(),
                 download->GetId(),
                 callback);
  DownloadTargetDeterminer::Start(
      download,
      GetPlatformDownloadPath(profile_, download, PLATFORM_TARGET_PATH),
      kDefaultPlatformConflictAction, download_prefs_.get(), this,
      target_determined_callback);
  return true;
}
