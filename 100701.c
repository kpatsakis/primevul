void OmniboxEditModel::ClearKeyword(const base::string16& visible_text) {
  autocomplete_controller()->Stop(false);
  omnibox_controller_->ClearPopupKeywordMode();

  const base::string16 window_text(keyword_ + visible_text);

  if (just_deleted_text_ || !visible_text.empty() ||
      !(popup_model() && popup_model()->IsOpen())) {
    view_->OnBeforePossibleChange();
    view_->SetWindowTextAndCaretPos(window_text.c_str(), keyword_.length(),
        false, false);
    keyword_.clear();
    is_keyword_hint_ = false;
    view_->OnAfterPossibleChange();
    just_deleted_text_ = true;  // OnAfterPossibleChange() fails to clear this
  } else {
    is_keyword_hint_ = true;
    view_->SetWindowTextAndCaretPos(window_text.c_str(), keyword_.length(),
        false, true);
  }
}
