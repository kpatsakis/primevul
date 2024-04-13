void PasswordAutofillAgent::HidePopup() {
  if (autofill_agent_)
    autofill_agent_->GetAutofillDriver()->HidePopup();
}
