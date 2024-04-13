void WebContentsImpl::UpdateTitle(RenderViewHost* rvh,
                                  int32 page_id,
                                  const string16& title,
                                  base::i18n::TextDirection title_direction) {
  SetNotWaitingForResponse();

  NavigationEntryImpl* entry = controller_.GetEntryWithPageID(
      rvh->GetSiteInstance(), page_id);

  if (!entry && rvh != GetRenderViewHost())
    return;

  if (!UpdateTitleForEntry(entry, title))
    return;

  if (entry == controller_.GetEntryAtOffset(0))
    NotifyNavigationStateChanged(INVALIDATE_TYPE_TITLE);
}
