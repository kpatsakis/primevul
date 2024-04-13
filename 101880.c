void ProfilingProcessHost::ReportMetrics() {
  UMA_HISTOGRAM_ENUMERATION("OutOfProcessHeapProfiling.ProfilingMode", mode(),
                            Mode::kCount);
}
