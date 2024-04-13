base::StackSamplingProfiler::SamplingParams GetJankTimeBombSamplingParams() {
  base::StackSamplingProfiler::SamplingParams params;
  params.initial_delay = base::TimeDelta::FromMilliseconds(0);
  params.bursts = 1;
  params.samples_per_burst = 50;
  params.sampling_interval = base::TimeDelta::FromMilliseconds(100);
  return params;
}
