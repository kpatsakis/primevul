bool OmniboxEditModel::AcceptKeyword(EnteredKeywordModeMethod entered_method) {
  DCHECK(is_keyword_hint_ && !keyword_.empty());

  autocomplete_controller()->Stop(false);
  is_keyword_hint_ = false;

  if (popup_model() && popup_model()->IsOpen())
    popup_model()->SetSelectedLineState(OmniboxPopupModel::KEYWORD);
  else
    StartAutocomplete(false, true);

  bool save_original_selection = !has_temporary_text_;
  has_temporary_text_ = true;
  view_->OnTemporaryTextMaybeChanged(
      DisplayTextFromUserText(CurrentMatch(NULL).fill_into_edit),
      save_original_selection, true);

  content::RecordAction(base::UserMetricsAction("AcceptedKeywordHint"));
  UMA_HISTOGRAM_ENUMERATION(kEnteredKeywordModeHistogram, entered_method,
                            ENTERED_KEYWORD_MODE_NUM_ITEMS);

  return true;
}
