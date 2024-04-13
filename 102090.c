void PasswordAutofillAgent::UpdateStateForTextChange(
    const blink::WebInputElement& element) {
  blink::WebInputElement mutable_element = element;  // We need a non-const.

  if (element.isTextField())
    nonscript_modified_values_[element] = element.value();

  blink::WebFrame* const element_frame = element.document().frame();
  if (!element_frame)
    return;
  DCHECK_EQ(element_frame, render_frame()->GetWebFrame());

  std::unique_ptr<PasswordForm> password_form;
  if (element.form().isNull()) {
    password_form = CreatePasswordFormFromUnownedInputElements(
        *element_frame, &nonscript_modified_values_, &form_predictions_);
  } else {
    password_form = CreatePasswordFormFromWebForm(
        element.form(), &nonscript_modified_values_, &form_predictions_);
  }
  ProvisionallySavePassword(std::move(password_form), RESTRICTION_NONE);

  if (element.isPasswordField()) {
    PasswordToLoginMap::iterator iter = password_to_username_.find(element);
    if (iter != password_to_username_.end()) {
      web_input_to_password_info_[iter->second].password_was_edited_last = true;
      mutable_element.setAutofilled(false);
    }
  }
}
