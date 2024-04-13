std::wstring ChannelFromAdditionalParameters(const InstallConstants& mode,
                                             const std::wstring& ap_value) {
  assert(kUseGoogleUpdateIntegration);

  static constexpr wchar_t kChromeChannelBetaPattern[] = L"1?1-*";
  static constexpr wchar_t kChromeChannelBetaX64Pattern[] = L"*x64-beta*";
  static constexpr wchar_t kChromeChannelDevPattern[] = L"2?0-d*";
  static constexpr wchar_t kChromeChannelDevX64Pattern[] = L"*x64-dev*";

  std::wstring value;
  value.reserve(ap_value.size());
  std::transform(ap_value.begin(), ap_value.end(), std::back_inserter(value),
                 ::tolower);

  if (value.empty() ||
      (value.find(kChromeChannelStableExplicit) != std::wstring::npos)) {
    return std::wstring();
  }
  if (MatchPattern(value, kChromeChannelDevPattern) ||
      MatchPattern(value, kChromeChannelDevX64Pattern)) {
    return kChromeChannelDev;
  }
  if (MatchPattern(value, kChromeChannelBetaPattern) ||
      MatchPattern(value, kChromeChannelBetaX64Pattern)) {
    return kChromeChannelBeta;
  }
  return std::wstring();
}
