void WebContentsImpl::OnSavePage() {
  if (!IsSavable()) {
    RecordDownloadSource(INITIATED_BY_SAVE_PACKAGE_ON_NON_HTML);
    SaveFrame(GetURL(), Referrer());
    return;
  }

  Stop();

  save_package_ = new SavePackage(this);
  save_package_->GetSaveInfo();
}
