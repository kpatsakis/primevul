bool OmniboxEditModel::CurrentTextIsURL() const {
  if (controller_->GetToolbarModel()->WouldReplaceURL())
    return false;

  if (!user_input_in_progress_)
    return true;

  return !AutocompleteMatch::IsSearchType(CurrentMatch(NULL).type);
}
