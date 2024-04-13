bool OmniboxEditModel::CommitSuggestedText() {
  const base::string16 suggestion = view_->GetGrayTextAutocompletion();
  if (suggestion.empty())
    return false;

  const base::string16 final_text = view_->GetText() + suggestion;
  view_->OnBeforePossibleChange();
  view_->SetWindowTextAndCaretPos(final_text, final_text.length(), false,
      false);
  view_->OnAfterPossibleChange();
  return true;
}
