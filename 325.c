void RefreshElevationPolicy() {
  const wchar_t kIEFrameDll[] = L"ieframe.dll";
  const char kIERefreshPolicy[] = "IERefreshElevationPolicy";

  HMODULE ieframe = LoadLibrary(kIEFrameDll);
  if (ieframe) {
    typedef HRESULT (__stdcall *IERefreshPolicy)();
    IERefreshPolicy ie_refresh_policy = reinterpret_cast<IERefreshPolicy>(
        GetProcAddress(ieframe, kIERefreshPolicy));

    if (ie_refresh_policy) {
      ie_refresh_policy();
    } else {
      VLOG(1) << kIERefreshPolicy << " not supported.";
    }

    FreeLibrary(ieframe);
  } else {
    VLOG(1) << "Cannot load " << kIEFrameDll;
  }
}
