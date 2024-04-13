void OmniboxEditModel::OnCurrentMatchChanged() {
  has_temporary_text_ = false;

  const AutocompleteMatch& match = omnibox_controller_->current_match();

  base::string16 keyword;
  bool is_keyword_hint;
  match.GetKeywordUIState(profile_, &keyword, &is_keyword_hint);
  if (popup_model())
    popup_model()->OnResultChanged();
  const base::string16 inline_autocompletion(match.inline_autocompletion);
  OnPopupDataChanged(inline_autocompletion, NULL, keyword, is_keyword_hint);
}
