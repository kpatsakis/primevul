views::View* ExtensionInstallDialogView::CreateExtraView() {
  if (!prompt_->has_webstore_data())
    return nullptr;

  views::Link* store_link = new views::Link(
      l10n_util::GetStringUTF16(IDS_EXTENSION_PROMPT_STORE_LINK));
  store_link->set_listener(this);
   return store_link;
 }
