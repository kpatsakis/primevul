bool NavigationRequest::IsAllowedByCSPDirective(
    CSPContext* context,
    CSPDirective::Name directive,
    bool has_followed_redirect,
    bool url_upgraded_after_redirect,
    bool is_response_check,
    CSPContext::CheckCSPDisposition disposition) {
  GURL url;
  if (url_upgraded_after_redirect &&
      disposition == CSPContext::CheckCSPDisposition::CHECK_REPORT_ONLY_CSP &&
      common_params_.url.SchemeIs(url::kHttpsScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(url::kHttpScheme);
    url = common_params_.url.ReplaceComponents(replacements);
  } else {
    url = common_params_.url;
  }
  return context->IsAllowedByCsp(
      directive, url, has_followed_redirect, is_response_check,
      common_params_.source_location.value_or(SourceLocation()), disposition,
      begin_params_->is_form_submission);
}
