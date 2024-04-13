void PasswordAutofillAgent::SendPasswordForms(bool only_visible) {
  std::unique_ptr<RendererSavePasswordProgressLogger> logger;
  if (logging_state_active_) {
    logger.reset(new RendererSavePasswordProgressLogger(this, routing_id()));
    logger->LogMessage(Logger::STRING_SEND_PASSWORD_FORMS_METHOD);
    logger->LogBoolean(Logger::STRING_ONLY_VISIBLE, only_visible);
  }

  blink::WebFrame* frame = render_frame()->GetWebFrame();
  blink::WebSecurityOrigin origin = frame->document().getSecurityOrigin();
  if (logger) {
    logger->LogURL(Logger::STRING_SECURITY_ORIGIN,
                   GURL(origin.toString().utf8()));
  }
  if (!OriginCanAccessPasswordManager(origin)) {
    if (logger) {
      logger->LogMessage(Logger::STRING_SECURITY_ORIGIN_FAILURE);
    }
    return;
  }

  if (form_util::IsWebpageEmpty(frame)) {
    if (logger) {
      logger->LogMessage(Logger::STRING_WEBPAGE_EMPTY);
    }
    return;
  }

  blink::WebVector<blink::WebFormElement> forms;
  frame->document().forms(forms);
  if (logger)
    logger->LogNumber(Logger::STRING_NUMBER_OF_ALL_FORMS, forms.size());

  std::vector<PasswordForm> password_forms;
  for (const blink::WebFormElement& form : forms) {
    if (only_visible) {
      bool is_form_visible = form_util::AreFormContentsVisible(form);
      if (logger) {
        LogHTMLForm(logger.get(), Logger::STRING_FORM_FOUND_ON_PAGE, form);
        logger->LogBoolean(Logger::STRING_FORM_IS_VISIBLE, is_form_visible);
      }

      if (!is_form_visible)
        continue;
    }

    std::unique_ptr<PasswordForm> password_form(
        CreatePasswordFormFromWebForm(form, nullptr, &form_predictions_));
    if (password_form) {
      if (logger) {
        logger->LogPasswordForm(Logger::STRING_FORM_IS_PASSWORD,
                                *password_form);
      }
      password_forms.push_back(*password_form);
    }
  }

  bool add_unowned_inputs = true;
  if (only_visible) {
    std::vector<blink::WebFormControlElement> control_elements =
        form_util::GetUnownedAutofillableFormFieldElements(
            frame->document().all(), nullptr);
    add_unowned_inputs =
        form_util::IsSomeControlElementVisible(control_elements);
    if (logger) {
      logger->LogBoolean(Logger::STRING_UNOWNED_INPUTS_VISIBLE,
                         add_unowned_inputs);
    }
  }
  if (add_unowned_inputs) {
    std::unique_ptr<PasswordForm> password_form(
        CreatePasswordFormFromUnownedInputElements(*frame, nullptr,
                                                   &form_predictions_));
    if (password_form) {
      if (logger) {
        logger->LogPasswordForm(Logger::STRING_FORM_IS_PASSWORD,
                                *password_form);
      }
      password_forms.push_back(*password_form);
    }
  }

  if (password_forms.empty() && !only_visible) {
    return;
  }

  if (only_visible) {
    blink::WebFrame* main_frame = render_frame()->GetWebFrame()->top();
    bool did_stop_loading = !main_frame || !main_frame->isLoading();
    Send(new AutofillHostMsg_PasswordFormsRendered(routing_id(), password_forms,
                                                   did_stop_loading));
  } else {
    Send(new AutofillHostMsg_PasswordFormsParsed(routing_id(), password_forms));
  }
}
