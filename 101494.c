std::unique_ptr<PasswordForm> PasswordAutofillAgent::GetPasswordFormFromWebForm(
    const WebFormElement& web_form) {
  return CreateSimplifiedPasswordFormFromWebForm(web_form, &field_data_manager_,
                                                 &username_detector_cache_);
}
