void RendererSchedulerImpl::OnReportFineGrainedExpectedQueueingTime(
    const char* split_description,
    base::TimeDelta queueing_time) {
  base::UmaHistogramCustomCounts(
      split_description, queueing_time.InMicroseconds(),
      kMinExpectedQueueingTimeBucket, kMaxExpectedQueueingTimeBucket,
      kNumberExpectedQueueingTimeBuckets);
}
