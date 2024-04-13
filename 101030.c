void Instance::NavigateTo(const std::string& url, bool open_in_new_tab) {
  std::string url_copy(url);

  if (!url_copy.empty()) {
    if (url_copy[0] == '#') {
      url_copy = url_ + url_copy;
      if (!open_in_new_tab) {
        int page_number = GetInitialPage(url_copy);
        if (page_number >= 0)
          ScrollToPage(page_number);
      }
    }
    if (url_copy.find("://") == std::string::npos &&
        url_copy.find("mailto:") == std::string::npos) {
      url_copy = "http://" + url_copy;
    }
    if (url_copy.find("http://") != 0 &&
        url_copy.find("https://") != 0 &&
        url_copy.find("ftp://") != 0 &&
        url_copy.find("file://") != 0 &&
        url_copy.find("mailto:") != 0) {
      return;
    }
    if (url_copy == "http://" ||
        url_copy == "https://" ||
        url_copy == "ftp://" ||
        url_copy == "file://" ||
        url_copy == "mailto:") {
      return;
    }
  }
  if (open_in_new_tab) {
    GetWindowObject().Call("open", url_copy);
  } else {
    GetWindowObject().GetProperty("top").GetProperty("location").
        SetProperty("href", url_copy);
  }
}
