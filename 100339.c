void PluginObserver::OnMissingPluginStatus(int status) {
#if defined(OS_WIN)
  if (status == webkit::npapi::default_plugin::MISSING_PLUGIN_AVAILABLE) {
    tab_contents_->AddInfoBar(
        new PluginInstallerInfoBarDelegate(tab_contents()));
    return;
  }

  DCHECK_EQ(webkit::npapi::default_plugin::MISSING_PLUGIN_USER_STARTED_DOWNLOAD,
            status);
  for (size_t i = 0; i < tab_contents_->infobar_count(); ++i) {
    InfoBarDelegate* delegate = tab_contents_->GetInfoBarDelegateAt(i);
    if (delegate->AsPluginInstallerInfoBarDelegate() != NULL) {
      tab_contents_->RemoveInfoBar(delegate);
      return;
    }
  }
#endif
}
