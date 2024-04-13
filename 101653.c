std::unique_ptr<NavigationRequest> NavigationRequest::CreateRendererInitiated(
    FrameTreeNode* frame_tree_node,
    NavigationEntryImpl* entry,
    const CommonNavigationParams& common_params,
    mojom::BeginNavigationParamsPtr begin_params,
    int current_history_list_offset,
    int current_history_list_length,
    bool override_user_agent,
    scoped_refptr<network::SharedURLLoaderFactory> blob_url_loader_factory) {
  DCHECK(FrameMsg_Navigate_Type::IsReload(common_params.navigation_type) ||
         common_params.navigation_type ==
             FrameMsg_Navigate_Type::DIFFERENT_DOCUMENT);

  RequestNavigationParams request_params(
      override_user_agent,
      std::vector<GURL>(),  // redirects
      common_params.url, common_params.method,
      false,                          // can_load_local_resources
      PageState(),                    // page_state
      0,                              // nav_entry_id
      false,                          // is_history_navigation_in_new_child
      std::map<std::string, bool>(),  // subframe_unique_names
      false,  // intended_as_new_entry
      -1,     // |pending_history_list_offset| is set to -1 because
      current_history_list_offset, current_history_list_length,
      false,  // is_view_source
      false /*should_clear_history_list*/);
  std::unique_ptr<NavigationRequest> navigation_request(new NavigationRequest(
      frame_tree_node, common_params, std::move(begin_params), request_params,
      false,  // browser_initiated
      true,   // from_begin_navigation
      nullptr, entry,
      nullptr));  // navigation_ui_data
  navigation_request->blob_url_loader_factory_ =
      std::move(blob_url_loader_factory);
  return navigation_request;
}
