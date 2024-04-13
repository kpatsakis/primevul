bool GetBrowserFromWindowID(
    UIThreadExtensionFunction* function, int window_id, Browser** browser) {
  if (window_id == extension_misc::kCurrentWindowId) {
    *browser = function->GetCurrentBrowser();
    if (!(*browser) || !(*browser)->window()) {
      function->SetError(keys::kNoCurrentWindowError);
      return false;
    }
  } else {
    std::string error;
    *browser = GetBrowserInProfileWithId(
        function->profile(), window_id, function->include_incognito(), &error);
    if (!*browser) {
      function->SetError(error);
      return false;
    }
  }
  return true;
}
