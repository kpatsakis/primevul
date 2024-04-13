bool PasswordAutofillAgent::ShowSuggestionPopup(
    const PasswordInfo& password_info,
    const blink::WebInputElement& user_input,
    bool show_all,
    bool show_on_password_field) {
  DCHECK(!user_input.isNull());
  blink::WebFrame* frame = user_input.document().frame();
  if (!frame)
    return false;

  blink::WebView* webview = frame->view();
  if (!webview)
    return false;

  if (user_input.isPasswordField() && !user_input.isAutofilled() &&
      !user_input.value().isEmpty()) {
    Send(new AutofillHostMsg_HidePopup(routing_id()));
    return false;
  }

  FormData form;
  FormFieldData field;
  form_util::FindFormAndFieldForFormControlElement(user_input, &form, &field);

  int options = 0;
  if (show_all)
    options |= SHOW_ALL;
  if (show_on_password_field)
    options |= IS_PASSWORD_FIELD;

  base::string16 username_string(
      user_input.isPasswordField()
          ? base::string16()
          : static_cast<base::string16>(user_input.value()));

  Send(new AutofillHostMsg_ShowPasswordSuggestions(
      routing_id(), password_info.key, field.text_direction, username_string,
      options,
      render_frame()->GetRenderView()->ElementBoundsInWindow(user_input)));
  username_query_prefix_ = username_string;
  return CanShowSuggestion(password_info.fill_data, username_string, show_all);
}
