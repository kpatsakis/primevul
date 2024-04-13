void ChromeDownloadManagerDelegate::CheckDownloadUrlDone(
    int32 download_id,
    const content::DownloadTargetCallback& callback,
    DownloadProtectionService::DownloadCheckResult result) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download = download_manager_->GetDownload(download_id);
  if (!download || (download->GetState() != DownloadItem::IN_PROGRESS))
    return;

  VLOG(2) << __FUNCTION__ << "() download = " << download->DebugString(false)
          << " verdict = " << result;
  content::DownloadDangerType danger_type = download->GetDangerType();
  if (result != DownloadProtectionService::SAFE)
    danger_type = content::DOWNLOAD_DANGER_TYPE_DANGEROUS_URL;

  HistoryService* history = HistoryServiceFactory::GetForProfile(
      profile_, Profile::EXPLICIT_ACCESS);
  if (!history || !download->GetReferrerUrl().is_valid()) {
    CheckVisitedReferrerBeforeDone(download_id, callback, danger_type, false);
    return;
  }
  history->GetVisibleVisitCountToHost(
      download->GetReferrerUrl(), &history_consumer_,
      base::Bind(&VisitCountsToVisitedBefore, base::Bind(
          &ChromeDownloadManagerDelegate::CheckVisitedReferrerBeforeDone,
          this, download_id, callback, danger_type)));
}
