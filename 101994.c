void NavigationControllerImpl::RendererDidNavigateToNewPage(
    RenderFrameHostImpl* rfh,
    const FrameHostMsg_DidCommitProvisionalLoad_Params& params,
    bool is_same_document,
    bool replace_entry,
    NavigationHandleImpl* handle) {
  std::unique_ptr<NavigationEntryImpl> new_entry;
  bool update_virtual_url = false;

  if (is_same_document && GetLastCommittedEntry()) {
    FrameNavigationEntry* frame_entry = new FrameNavigationEntry(
        rfh->frame_tree_node()->unique_name(), params.item_sequence_number,
        params.document_sequence_number, rfh->GetSiteInstance(), nullptr,
        params.url, params.referrer, params.redirects, params.page_state,
        params.method, params.post_id, nullptr /* blob_url_loader_factory */);

    new_entry = GetLastCommittedEntry()->CloneAndReplace(
        frame_entry, true, rfh->frame_tree_node(),
        delegate_->GetFrameTree()->root());
    if (new_entry->GetURL().GetOrigin() != params.url.GetOrigin()) {
      new_entry->GetSSL() = SSLStatus();

      if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
          handle->GetNetErrorCode() == net::OK) {
        UMA_HISTOGRAM_BOOLEAN(
            "Navigation.SecureSchemeHasSSLStatus.NewPageInPageOriginMismatch",
            !!new_entry->GetSSL().certificate);
      }
    }

    CHECK(frame_entry->HasOneRef());

    update_virtual_url = new_entry->update_virtual_url_with_url();

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      UMA_HISTOGRAM_BOOLEAN("Navigation.SecureSchemeHasSSLStatus.NewPageInPage",
                            !!new_entry->GetSSL().certificate);
    }
  }

  if (!new_entry &&
      PendingEntryMatchesHandle(handle) && pending_entry_index_ == -1 &&
      (!pending_entry_->site_instance() ||
       pending_entry_->site_instance() == rfh->GetSiteInstance())) {
    new_entry = pending_entry_->Clone();

    update_virtual_url = new_entry->update_virtual_url_with_url();
    new_entry->GetSSL() = SSLStatus(handle->GetSSLInfo());

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      UMA_HISTOGRAM_BOOLEAN(
          "Navigation.SecureSchemeHasSSLStatus.NewPagePendingEntryMatches",
          !!new_entry->GetSSL().certificate);
    }
  }

  if (!new_entry) {
    new_entry = base::WrapUnique(new NavigationEntryImpl);

    GURL url = params.url;
    bool needs_update = false;
    BrowserURLHandlerImpl::GetInstance()->RewriteURLIfNecessary(
        &url, browser_context_, &needs_update);
    new_entry->set_update_virtual_url_with_url(needs_update);

    update_virtual_url = needs_update;
    new_entry->GetSSL() = SSLStatus(handle->GetSSLInfo());

    if (params.url.SchemeIs(url::kHttpsScheme) && !rfh->GetParent() &&
        handle->GetNetErrorCode() == net::OK) {
      UMA_HISTOGRAM_BOOLEAN(
          "Navigation.SecureSchemeHasSSLStatus.NewPageNoMatchingEntry",
          !!new_entry->GetSSL().certificate);
    }
  }

  new_entry->set_page_type(params.url_is_unreachable ? PAGE_TYPE_ERROR
                                                     : PAGE_TYPE_NORMAL);
  new_entry->SetURL(params.url);
  if (update_virtual_url)
    UpdateVirtualURLToURL(new_entry.get(), params.url);
  new_entry->SetReferrer(params.referrer);
  new_entry->SetTransitionType(params.transition);
  new_entry->set_site_instance(
      static_cast<SiteInstanceImpl*>(rfh->GetSiteInstance()));
  new_entry->SetOriginalRequestURL(params.original_request_url);
  new_entry->SetIsOverridingUserAgent(params.is_overriding_user_agent);

  FrameNavigationEntry* frame_entry =
      new_entry->GetFrameEntry(rfh->frame_tree_node());
  frame_entry->set_frame_unique_name(rfh->frame_tree_node()->unique_name());
  frame_entry->set_item_sequence_number(params.item_sequence_number);
  frame_entry->set_document_sequence_number(params.document_sequence_number);
  frame_entry->set_redirect_chain(params.redirects);
  frame_entry->SetPageState(params.page_state);
  frame_entry->set_method(params.method);
  frame_entry->set_post_id(params.post_id);

  if (is_same_document && GetLastCommittedEntry()) {
    new_entry->SetTitle(GetLastCommittedEntry()->GetTitle());
    new_entry->GetFavicon() = GetLastCommittedEntry()->GetFavicon();
  }

  DCHECK(!params.history_list_was_cleared || !replace_entry);
  if (params.history_list_was_cleared) {
    DiscardNonCommittedEntriesInternal();
    entries_.clear();
    last_committed_entry_index_ = -1;
  }

  InsertOrReplaceEntry(std::move(new_entry), replace_entry);
}
