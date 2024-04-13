TestNavigationManager::TestNavigationManager(WebContents* web_contents,
                                             const GURL& url)
    : WebContentsObserver(web_contents),
      url_(url),
      handle_(nullptr),
      navigation_paused_(false),
      current_state_(NavigationState::INITIAL),
      desired_state_(NavigationState::STARTED),
      weak_factory_(this) {}
