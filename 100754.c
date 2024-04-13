bool DelegatedFrameHostClient::ShouldCreateResizeLock() {
#if !defined(OS_CHROMEOS)
  return false;
#else
  return GetDelegatedFrameHost()->ShouldCreateResizeLock();
#endif
}
