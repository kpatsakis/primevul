void PasswordAutofillAgent::ProvisionallySavePassword(
    std::unique_ptr<PasswordForm> password_form,
    ProvisionallySaveRestriction restriction) {
  if (!password_form || (restriction == RESTRICTION_NON_EMPTY_PASSWORD &&
                         password_form->password_value.empty() &&
                         password_form->new_password_value.empty())) {
    return;
  }
  provisionally_saved_form_ = std::move(password_form);
}
