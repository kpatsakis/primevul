bool Extension::LoadLaunchURL(const DictionaryValue* manifest,
                              std::string* error) {
  Value* temp = NULL;

  if (manifest->Get(keys::kLaunchLocalPath, &temp)) {
    if (manifest->Get(keys::kLaunchWebURL, NULL)) {
      *error = errors::kLaunchPathAndURLAreExclusive;
      return false;
    }

    std::string launch_path;
    if (!temp->GetAsString(&launch_path)) {
      *error = errors::kInvalidLaunchLocalPath;
      return false;
    }

    GURL resolved = url().Resolve(launch_path);
    if (!resolved.is_valid() || resolved.GetOrigin() != url()) {
      *error = errors::kInvalidLaunchLocalPath;
      return false;
    }

    launch_local_path_ = launch_path;
  } else if (manifest->Get(keys::kLaunchWebURL, &temp)) {
    std::string launch_url;
    if (!temp->GetAsString(&launch_url)) {
      *error = errors::kInvalidLaunchWebURL;
      return false;
    }

    GURL url(launch_url);
    URLPattern pattern(kValidWebExtentSchemes);
    if (!url.is_valid() || !pattern.SetScheme(url.scheme())) {
      *error = errors::kInvalidLaunchWebURL;
      return false;
    }

    launch_web_url_ = launch_url;
  } else if (is_app()) {
    *error = errors::kLaunchURLRequired;
    return false;
  }

  if (web_extent().is_empty() && !launch_web_url().empty()) {
    GURL launch_url(launch_web_url());
    URLPattern pattern(kValidWebExtentSchemes);
    if (!pattern.SetScheme("*")) {
      *error = errors::kInvalidLaunchWebURL;
      return false;
    }
    pattern.set_host(launch_url.host());
    pattern.SetPath("/*");
    extent_.AddPattern(pattern);
  }

  if (id() == extension_misc::kWebStoreAppId) {
    std::string gallery_url_str = CommandLine::ForCurrentProcess()->
        GetSwitchValueASCII(switches::kAppsGalleryURL);

    if (!gallery_url_str.empty()) {
      GURL gallery_url(gallery_url_str);
      if (!gallery_url.is_valid()) {
        LOG(WARNING) << "Invalid url given in switch "
                     << switches::kAppsGalleryURL;
      } else {
        if (gallery_url.has_port()) {
          LOG(WARNING) << "URLs passed to switch " << switches::kAppsGalleryURL
                       << " should not contain a port.  Removing it.";

          GURL::Replacements remove_port;
          remove_port.ClearPort();
          gallery_url = gallery_url.ReplaceComponents(remove_port);
        }

        launch_web_url_ = gallery_url.spec();

        URLPattern pattern(kValidWebExtentSchemes);
        pattern.Parse(gallery_url.spec(), URLPattern::PARSE_STRICT);
        pattern.SetPath(pattern.path() + '*');
        extent_.AddPattern(pattern);
      }
    }
  }

  return true;
}
