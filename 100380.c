void PromptBrowserLoginFunction::Observe(NotificationType type,
                                         const NotificationSource& source,
                                         const NotificationDetails& details) {
  std::string service;
  if (type == NotificationType::TOKEN_AVAILABLE) {
    TokenService::TokenAvailableDetails* available =
        Details<TokenService::TokenAvailableDetails>(details).ptr();
    service = available->service();
  } else if (type == NotificationType::TOKEN_REQUEST_FAILED) {
    TokenService::TokenRequestFailedDetails* failed =
        Details<TokenService::TokenRequestFailedDetails>(details).ptr();
    service = failed->service();
  } else {
    NOTREACHED();
  }

  if (service != GaiaConstants::kGaiaService) {
    return;
  }

  DCHECK(waiting_for_token_);

  result_.reset(CreateLoginResult(GetDefaultProfile(profile_)));
  SendResponse(true);

  Release();
}
