void OmniboxViewWin::UpdatePopup() {
  ScopedFreeze freeze(this, GetTextObjectModel());
  model_->SetInputInProgress(true);

  if (ime_candidate_window_open_)
    return;

  if (!model_->has_focus()) {
    return;
  }

  CHARRANGE sel;
  GetSel(sel);
  model_->StartAutocomplete(sel.cpMax != sel.cpMin,
                            (sel.cpMax < GetTextLength()) || IsImeComposing());
}
