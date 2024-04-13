std::unique_ptr<NavigationRequest> NavigationRequest::CreateBrowserInitiated(
    FrameTreeNode* frame_tree_node,
    const GURL& dest_url,
    const Referrer& dest_referrer,
    const FrameNavigationEntry& frame_entry,
    const NavigationEntryImpl& entry,
    FrameMsg_Navigate_Type::Value navigation_type,
    PreviewsState previews_state,
    bool is_same_document_history_load,
    bool is_history_navigation_in_new_child,
    const scoped_refptr<network::ResourceRequestBody>& post_body,
    base::TimeTicks navigation_start,
    NavigationControllerImpl* controller,
    std::unique_ptr<NavigationUIData> navigation_ui_data,
    base::TimeTicks input_start) {
  scoped_refptr<network::ResourceRequestBody> request_body;
  std::string post_content_type;
  if (post_body) {
    request_body = post_body;
  } else if (frame_entry.method() == "POST") {
    request_body = frame_entry.GetPostData(&post_content_type);
    post_content_type =
        base::TrimWhitespaceASCII(post_content_type, base::TRIM_ALL)
            .as_string();
  }
  bool is_form_submission = !!request_body;

  base::Optional<url::Origin> initiator =
      frame_tree_node->IsMainFrame()
          ? base::Optional<url::Origin>()
          : base::Optional<url::Origin>(
                frame_tree_node->frame_tree()->root()->current_origin());

  bool browser_initiated = !entry.is_renderer_initiated();

  CommonNavigationParams common_params = entry.ConstructCommonNavigationParams(
      frame_entry, request_body, dest_url, dest_referrer, navigation_type,
      previews_state, navigation_start, input_start);

  RequestNavigationParams request_params =
      entry.ConstructRequestNavigationParams(
          frame_entry, common_params.url, common_params.method,
          is_history_navigation_in_new_child,
          entry.GetSubframeUniqueNames(frame_tree_node),
          controller->GetPendingEntryIndex() == -1,
          controller->GetIndexOfEntry(&entry),
          controller->GetLastCommittedEntryIndex(),
          controller->GetEntryCount());
  request_params.post_content_type = post_content_type;

  std::unique_ptr<NavigationRequest> navigation_request(new NavigationRequest(
      frame_tree_node, common_params,
      mojom::BeginNavigationParams::New(
          entry.extra_headers(), net::LOAD_NORMAL,
          false /* skip_service_worker */, REQUEST_CONTEXT_TYPE_LOCATION,
          blink::WebMixedContentContextType::kBlockable, is_form_submission,
          GURL() /* searchable_form_url */,
          std::string() /* searchable_form_encoding */, initiator,
          GURL() /* client_side_redirect_url */,
          base::nullopt /* devtools_initiator_info */),
      request_params, browser_initiated, false /* from_begin_navigation */,
      &frame_entry, &entry, std::move(navigation_ui_data), nullptr));
  navigation_request->blob_url_loader_factory_ =
      frame_entry.blob_url_loader_factory();
  return navigation_request;
}
