void WebContentsImpl::DidNavigateAnyFramePostCommit(
    RenderViewHost* render_view_host,
    const LoadCommittedDetails& details,
    const ViewHostMsg_FrameNavigate_Params& params) {
  if (dialog_manager_ && !details.is_in_page)
    dialog_manager_->CancelActiveAndPendingDialogs(this);

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    DidNavigateAnyFrame(details, params));
}
