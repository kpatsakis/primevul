void PageHandler::NavigationReset(NavigationRequest* navigation_request) {
  auto navigate_callback = navigate_callbacks_.find(
      navigation_request->frame_tree_node()->devtools_frame_token());
  if (navigate_callback == navigate_callbacks_.end())
    return;
  std::string frame_id =
      navigation_request->frame_tree_node()->devtools_frame_token().ToString();
  bool success = navigation_request->net_error() == net::OK;
  std::string error_string =
      net::ErrorToString(navigation_request->net_error());
  navigate_callback->second->sendSuccess(
      frame_id,
      Maybe<std::string>(
          navigation_request->devtools_navigation_token().ToString()),
      success ? Maybe<std::string>() : Maybe<std::string>(error_string));
  navigate_callbacks_.erase(navigate_callback);
}
