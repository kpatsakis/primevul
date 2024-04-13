bool IsUnownedPasswordFormVisible(blink::WebFrame* frame,
                                  const GURL& action,
                                  const GURL& origin,
                                  const FormData& form_data,
                                  const FormsPredictionsMap& form_predictions) {
  std::unique_ptr<PasswordForm> unowned_password_form(
      CreatePasswordFormFromUnownedInputElements(*frame, nullptr,
                                                 &form_predictions));
  if (!unowned_password_form)
    return false;
  std::vector<blink::WebFormControlElement> control_elements =
      form_util::GetUnownedAutofillableFormFieldElements(
          frame->document().all(), nullptr);
  if (!form_util::IsSomeControlElementVisible(control_elements))
    return false;

#if !defined(OS_MACOSX) && !defined(OS_ANDROID)
  const bool action_is_empty = action == origin;
  bool forms_are_same =
      action_is_empty ? form_data.SameFormAs(unowned_password_form->form_data)
                      : action == unowned_password_form->action;
  return forms_are_same;
#else  // OS_MACOSX or OS_ANDROID
  return action == unowned_password_form->action;
#endif
}
