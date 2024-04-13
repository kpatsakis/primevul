bool OmniboxViewViews::MaybeFocusTabButton() {
  if (SelectedSuggestionHasTabMatch() &&
      model()->popup_model()->selected_line_state() ==
          OmniboxPopupModel::NORMAL) {
    model()->popup_model()->SetSelectedLineState(
        OmniboxPopupModel::BUTTON_FOCUSED);
    popup_view_->ProvideButtonFocusHint(
        model()->popup_model()->selected_line());
    return true;
  }
  return false;
}
