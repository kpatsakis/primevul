void PageHandler::DidRunBeforeUnloadConfirm(const GURL& url,
                                            bool has_non_devtools_handlers,
                                            JavaScriptDialogCallback callback) {
  if (!enabled_)
    return;
  DCHECK(pending_dialog_.is_null());
  pending_dialog_ = std::move(callback);
  frontend_->JavascriptDialogOpening(url.spec(), std::string(),
                                     Page::DialogTypeEnum::Beforeunload,
                                     has_non_devtools_handlers, std::string());
}
