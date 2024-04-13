version_info::Channel GetChromeChannel() {
#if defined(GOOGLE_CHROME_BUILD)
  std::wstring channel_name(GetChromeChannelName());
  if (channel_name.empty()) {
    return version_info::Channel::STABLE;
  }
  if (channel_name == L"beta") {
    return version_info::Channel::BETA;
  }
  if (channel_name == L"dev") {
    return version_info::Channel::DEV;
  }
  if (channel_name == L"canary") {
    return version_info::Channel::CANARY;
  }
#endif

  return version_info::Channel::UNKNOWN;
}
