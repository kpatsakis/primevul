void NavigatorImpl::RequestTransferURL(
    RenderFrameHostImpl* render_frame_host,
    const GURL& url,
    SiteInstance* source_site_instance,
    const std::vector<GURL>& redirect_chain,
    const Referrer& referrer,
    ui::PageTransition page_transition,
    const GlobalRequestID& transferred_global_request_id,
    bool should_replace_current_entry,
    const std::string& method,
    scoped_refptr<ResourceRequestBodyImpl> post_body,
    const std::string& extra_headers) {
  if (method != "POST" && post_body) {
    NOTREACHED();
    post_body = nullptr;
  }

  DCHECK(!render_frame_host->GetParent() ||
         SiteIsolationPolicy::AreCrossProcessFramesPossible());

  if (!delegate_->ShouldTransferNavigation(
          render_frame_host->frame_tree_node()->IsMainFrame()))
    return;

  GURL dest_url(url);
  Referrer referrer_to_use(referrer);
  FrameTreeNode* node = render_frame_host->frame_tree_node();
  SiteInstance* current_site_instance = render_frame_host->GetSiteInstance();
  if (!GetContentClient()->browser()->ShouldAllowOpenURL(current_site_instance,
                                                         url)) {
    dest_url = GURL(url::kAboutBlankURL);
  }

  bool is_renderer_initiated = true;
  if (render_frame_host->web_ui()) {
    if (ui::PageTransitionCoreTypeIs(page_transition, ui::PAGE_TRANSITION_LINK))
      page_transition = render_frame_host->web_ui()->GetLinkTransitionType();

    referrer_to_use = Referrer();

    is_renderer_initiated = false;
  }

  std::unique_ptr<NavigationEntryImpl> entry;
  if (!node->IsMainFrame()) {
    CHECK(SiteIsolationPolicy::UseSubframeNavigationEntries());
    if (controller_->GetLastCommittedEntry()) {
      entry = controller_->GetLastCommittedEntry()->Clone();
      entry->set_extra_headers(extra_headers);
    } else {
      entry = NavigationEntryImpl::FromNavigationEntry(
          controller_->CreateNavigationEntry(
              GURL(url::kAboutBlankURL), referrer_to_use, page_transition,
              is_renderer_initiated, extra_headers,
              controller_->GetBrowserContext()));
    }
    entry->AddOrUpdateFrameEntry(
        node, -1, -1, nullptr,
        static_cast<SiteInstanceImpl*>(source_site_instance),
        dest_url, referrer_to_use, redirect_chain, PageState(), method,
        -1);
  } else {
    entry = NavigationEntryImpl::FromNavigationEntry(
        controller_->CreateNavigationEntry(
            dest_url, referrer_to_use, page_transition, is_renderer_initiated,
            extra_headers, controller_->GetBrowserContext()));
    entry->root_node()->frame_entry->set_source_site_instance(
        static_cast<SiteInstanceImpl*>(source_site_instance));
    entry->SetRedirectChain(redirect_chain);
  }

  if (should_replace_current_entry && controller_->GetEntryCount() > 0)
    entry->set_should_replace_entry(true);
  if (controller_->GetLastCommittedEntry() &&
      controller_->GetLastCommittedEntry()->GetIsOverridingUserAgent()) {
    entry->SetIsOverridingUserAgent(true);
  }
  entry->set_transferred_global_request_id(transferred_global_request_id);

  scoped_refptr<FrameNavigationEntry> frame_entry(entry->GetFrameEntry(node));
  if (!frame_entry) {
    frame_entry = new FrameNavigationEntry(
        node->unique_name(), -1, -1, nullptr,
        static_cast<SiteInstanceImpl*>(source_site_instance), dest_url,
        referrer_to_use, method, -1);
  }
  NavigateToEntry(node, *frame_entry, *entry.get(), ReloadType::NONE, false,
                  false, false, post_body);
}
