void PasswordAutofillAgent::LogPrefilledUsernameFillOutcome(
    PrefilledUsernameFillOutcome outcome) {
  if (prefilled_username_metrics_logged_)
    return;
  prefilled_username_metrics_logged_ = true;
  UMA_HISTOGRAM_ENUMERATION("PasswordManager.PrefilledUsernameFillOutcome",
                            outcome);
}
