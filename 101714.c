void Release(FPDF_SYSFONTINFO* sysfontinfo) {
  auto* fontinfo_with_metrics =
      static_cast<FPDF_SYSFONTINFO_WITHMETRICS*>(sysfontinfo);
  if (!fontinfo_with_metrics->default_sysfontinfo->Release)
    return;
  fontinfo_with_metrics->default_sysfontinfo->Release(
      fontinfo_with_metrics->default_sysfontinfo);
}
