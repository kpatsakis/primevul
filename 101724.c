void DownloadItemImpl::ResumeInterruptedDownload(
    ResumptionRequestSource source) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if (state_ != INTERRUPTED_INTERNAL)
    return;

  DCHECK(!download_file_);
  weak_ptr_factory_.InvalidateWeakPtrs();

  ResumeMode mode = GetResumeMode();
  if (mode == RESUME_MODE_IMMEDIATE_RESTART ||
      mode == RESUME_MODE_USER_RESTART) {
    DCHECK(GetFullPath().empty());
    destination_info_.received_bytes = 0;
    last_modified_time_.clear();
    etag_.clear();
    destination_info_.hash.clear();
    hash_state_.reset();
    received_slices_.clear();
  }

  StoragePartition* storage_partition =
      BrowserContext::GetStoragePartitionForSite(GetBrowserContext(),
                                                 request_info_.site_url);

  net::NetworkTrafficAnnotationTag traffic_annotation =
      net::DefineNetworkTrafficAnnotation("download_manager_resume", R"(
        semantics {
          sender: "Download Manager"
          description:
            "When user resumes downloading a file, a network request is made "
            "to fetch it."
          trigger:
            "User resumes a download."
          data: "None."
          destination: WEBSITE
        }
        policy {
          cookies_allowed: YES
          cookies_store: "user"
          setting:
            "This feature cannot be disabled in settings, but it is activated "
            "by direct user action."
          chrome_policy {
            DownloadRestrictions {
              DownloadRestrictions: 3
            }
          }
        })");
  std::unique_ptr<DownloadUrlParameters> download_params(
      new DownloadUrlParameters(GetURL(),
                                storage_partition->GetURLRequestContext(),
                                traffic_annotation));
  download_params->set_file_path(GetFullPath());
  if (received_slices_.size() > 0) {
    std::vector<DownloadItem::ReceivedSlice> slices_to_download
        = FindSlicesToDownload(received_slices_);
    download_params->set_offset(slices_to_download[0].offset);
  } else {
    download_params->set_offset(GetReceivedBytes());
  }
  download_params->set_last_modified(GetLastModifiedTime());
  download_params->set_etag(GetETag());
  download_params->set_hash_of_partial_file(GetHash());
  download_params->set_hash_state(std::move(hash_state_));
  download_params->set_fetch_error_body(fetch_error_body_);

  auto* manager_delegate = GetBrowserContext()->GetDownloadManagerDelegate();
  if (manager_delegate) {
    download::InProgressCache* in_progress_cache =
        manager_delegate->GetInProgressCache();
    if (in_progress_cache) {
      base::Optional<download::DownloadEntry> entry =
          in_progress_cache->RetrieveEntry(GetGuid());
      if (entry)
        download_params->set_request_origin(entry.value().request_origin);
    }
  }

  download_params->set_referrer(
      Referrer(GetReferrerUrl(), blink::kWebReferrerPolicyAlways));

  TransitionTo(RESUMING_INTERNAL);
  RecordDownloadSource(source == ResumptionRequestSource::USER
                           ? INITIATED_BY_MANUAL_RESUMPTION
                           : INITIATED_BY_AUTOMATIC_RESUMPTION);
  delegate_->ResumeInterruptedDownload(std::move(download_params), GetId());

  if (job_)
    job_->Resume(false);
}
