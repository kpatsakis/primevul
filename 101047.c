void CorePageLoadMetricsObserver::RecordRappor(
    const page_load_metrics::PageLoadTiming& timing,
    const page_load_metrics::PageLoadExtraInfo& info) {
  if (g_browser_process->IsShuttingDown())
    return;
  rappor::RapporService* rappor_service = g_browser_process->rappor_service();
  if (!rappor_service)
    return;
  if (!info.time_to_commit)
    return;
  DCHECK(!info.committed_url.is_empty());
  if (!WasStartedInForegroundOptionalEventInForeground(
          timing.first_contentful_paint, info)) {
    return;
  }
  std::unique_ptr<rappor::Sample> sample =
      rappor_service->CreateSample(rappor::UMA_RAPPOR_TYPE);
  sample->SetStringField(
      "Domain", rappor::GetDomainAndRegistrySampleFromGURL(info.committed_url));
  uint64_t bucket_index =
      RapporHistogramBucketIndex(timing.first_contentful_paint.value());
  sample->SetFlagsField("Bucket", uint64_t(1) << bucket_index,
                        kNumRapporHistogramBuckets);
  sample->SetFlagsField(
      "IsSlow", timing.first_contentful_paint.value().InSecondsF() >= 10, 1);
  rappor_service->RecordSampleObj(internal::kRapporMetricsNameCoarseTiming,
                                  std::move(sample));
}
