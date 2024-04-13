void RenderView::willSubmitForm(WebFrame* frame, const WebFormElement& form) {
  NavigationState* navigation_state =
      NavigationState::FromDataSource(frame->provisionalDataSource());

  if (navigation_state->transition_type() == PageTransition::LINK)
    navigation_state->set_transition_type(PageTransition::FORM_SUBMIT);

  WebSearchableFormData web_searchable_form_data(form);
  navigation_state->set_searchable_form_url(web_searchable_form_data.url());
  navigation_state->set_searchable_form_encoding(
      web_searchable_form_data.encoding().utf8());
  PasswordForm* password_form_data =
      PasswordFormDomManager::CreatePasswordForm(form);
  navigation_state->set_password_form_data(password_form_data);

  if (password_form_data && password_form_data->password_value.empty()) {
    NavigationState* old_navigation_state =
        NavigationState::FromDataSource(frame->dataSource());
    if (old_navigation_state) {
      PasswordForm* old_form_data = old_navigation_state->password_form_data();
      if (old_form_data && old_form_data->action == password_form_data->action)
        password_form_data->password_value = old_form_data->password_value;
    }
  }

  FormData form_data;
  if (FormManager::WebFormElementToFormData(
          form, FormManager::REQUIRE_AUTOCOMPLETE, true, &form_data))
    Send(new ViewHostMsg_FormSubmitted(routing_id_, form_data));
}
