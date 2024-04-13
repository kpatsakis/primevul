void OmniboxEditModel::OpenMatch(AutocompleteMatch match,
                                 WindowOpenDisposition disposition,
                                 const GURL& alternate_nav_url,
                                 const base::string16& pasted_text,
                                 size_t index) {
  const base::TimeTicks& now(base::TimeTicks::Now());
  base::TimeDelta elapsed_time_since_user_first_modified_omnibox(
      now - time_user_first_modified_omnibox_);
  autocomplete_controller()->UpdateMatchDestinationURL(
      elapsed_time_since_user_first_modified_omnibox, &match);

  base::string16 input_text(pasted_text);
  if (input_text.empty())
      input_text = user_input_in_progress_ ? user_text_ : permanent_text_;
  scoped_ptr<OmniboxNavigationObserver> observer(
      new OmniboxNavigationObserver(
          profile_, input_text, match,
          autocomplete_controller()->history_url_provider()->SuggestExactInput(
              input_text, alternate_nav_url,
              AutocompleteInput::HasHTTPScheme(input_text))));

  base::TimeDelta elapsed_time_since_last_change_to_default_match(
      now - autocomplete_controller()->last_time_default_match_changed());
  DCHECK(match.provider);
  if ((match.provider->type() == AutocompleteProvider::TYPE_ZERO_SUGGEST) ||
      !popup_model()->IsOpen() || !pasted_text.empty()) {
    const base::TimeDelta default_time_delta =
        base::TimeDelta::FromMilliseconds(-1);
    elapsed_time_since_user_first_modified_omnibox = default_time_delta;
    elapsed_time_since_last_change_to_default_match = default_time_delta;
  }
  ACMatches fake_single_entry_matches;
  fake_single_entry_matches.push_back(match);
  AutocompleteResult fake_single_entry_result;
  fake_single_entry_result.AppendMatches(fake_single_entry_matches);
  OmniboxLog log(
      input_text,
      just_deleted_text_,
      input_.type(),
      popup_model()->IsOpen(),
      (!popup_model()->IsOpen() || !pasted_text.empty()) ? 0 : index,
      !pasted_text.empty(),
      -1,  // don't yet know tab ID; set later if appropriate
      ClassifyPage(),
      elapsed_time_since_user_first_modified_omnibox,
      match.inline_autocompletion.length(),
      elapsed_time_since_last_change_to_default_match,
      (!popup_model()->IsOpen() || !pasted_text.empty()) ?
          fake_single_entry_result : result());
  DCHECK(!popup_model()->IsOpen() || !pasted_text.empty() ||
         (log.elapsed_time_since_user_first_modified_omnibox >=
          log.elapsed_time_since_last_change_to_default_match))
      << "We should've got the notification that the user modified the "
      << "omnibox text at same time or before the most recent time the "
      << "default match changed.";

  if ((disposition == CURRENT_TAB) && delegate_->CurrentPageExists()) {
    log.tab_id = delegate_->GetSessionID().id();
  }
  autocomplete_controller()->AddProvidersInfo(&log.providers_info);
  content::NotificationService::current()->Notify(
      chrome::NOTIFICATION_OMNIBOX_OPENED_URL,
      content::Source<Profile>(profile_),
      content::Details<OmniboxLog>(&log));
  HISTOGRAM_ENUMERATION("Omnibox.EventCount", 1, 2);
  DCHECK(!last_omnibox_focus_.is_null())
      << "An omnibox focus should have occurred before opening a match.";
  UMA_HISTOGRAM_TIMES(kFocusToOpenTimeHistogram, now - last_omnibox_focus_);

  TemplateURL* template_url = match.GetTemplateURL(profile_, false);
  if (template_url) {
    if (match.transition == content::PAGE_TRANSITION_KEYWORD) {

      if (delegate_->ProcessExtensionKeyword(template_url, match,
                                             disposition)) {
        observer->OnSuccessfulNavigation();
        if (disposition != NEW_BACKGROUND_TAB)
          view_->RevertAll();
        return;
      }

      content::RecordAction(base::UserMetricsAction("AcceptedKeyword"));
      TemplateURLServiceFactory::GetForProfile(profile_)->IncrementUsageCount(
          template_url);
    } else {
      DCHECK_EQ(content::PAGE_TRANSITION_GENERATED, match.transition);
    }

    UMA_HISTOGRAM_ENUMERATION("Omnibox.SearchEngineType",
        TemplateURLPrepopulateData::GetEngineType(*template_url),
        SEARCH_ENGINE_MAX);
  }

  base::string16 current_text = view_->GetText();

  if (disposition != NEW_BACKGROUND_TAB) {
    base::AutoReset<bool> tmp(&in_revert_, true);
    view_->RevertAll();  // Revert the box to its unedited state.
  }

  if (match.type == AutocompleteMatchType::EXTENSION_APP) {
    ExtensionAppProvider::LaunchAppFromOmnibox(match, profile_, disposition);
    observer->OnSuccessfulNavigation();
  } else {
    RecordPercentageMatchHistogram(
        permanent_text_, current_text,
        controller_->GetToolbarModel()->WouldReplaceURL(),
        match.transition, view_->GetWidth());

    if (TemplateURLServiceFactory::GetForProfile(profile_)->
        IsSearchResultsPageFromDefaultSearchProvider(match.destination_url)) {
      content::RecordAction(
          base::UserMetricsAction("OmniboxDestinationURLIsSearchOnDSP"));
    }

    if (match.destination_url.is_valid()) {
      base::AutoReset<bool> tmp(&in_revert_, true);
      controller_->OnAutocompleteAccept(
          match.destination_url, disposition,
          content::PageTransitionFromInt(
              match.transition | content::PAGE_TRANSITION_FROM_ADDRESS_BAR));
      if (observer->load_state() != OmniboxNavigationObserver::LOAD_NOT_SEEN)
        ignore_result(observer.release());  // The observer will delete itself.
    }
  }

  if (match.starred)
    RecordBookmarkLaunch(NULL, BOOKMARK_LAUNCH_LOCATION_OMNIBOX);
}
