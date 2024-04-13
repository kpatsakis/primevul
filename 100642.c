void OneClickSigninHelper::NavigateToPendingEntry(
    const GURL& url,
    content::NavigationController::ReloadType reload_type) {
  VLOG(1) << "OneClickSigninHelper::NavigateToPendingEntry: url=" << url.spec();
  const GURL continue_url = signin::GetNextPageURLForPromoURL(
      signin::GetPromoURL(signin::SOURCE_START_PAGE, false));
  GURL::Replacements replacements;
  replacements.ClearQuery();

  if (!IsValidGaiaSigninRedirectOrResponseURL(url) &&
      continue_url_.is_valid() &&
      url.ReplaceComponents(replacements) !=
          continue_url_.ReplaceComponents(replacements)) {
    if (++untrusted_navigations_since_signin_visit_ > kMaxNavigationsSince)
      CleanTransientState();
  }
}
