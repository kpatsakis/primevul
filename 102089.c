bool PasswordAutofillAgent::TextDidChangeInTextField(
    const blink::WebInputElement& element) {
  blink::WebInputElement mutable_element = element;  // We need a non-const.
  mutable_element.setAutofilled(false);

  WebInputToPasswordInfoMap::iterator iter =
      web_input_to_password_info_.find(element);
  if (iter != web_input_to_password_info_.end()) {
    iter->second.password_was_edited_last = false;
    if (iter->second.fill_data.wait_for_username)
      return false;
  }

  return ShowSuggestions(element, false, false);
}
