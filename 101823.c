bool PrintRenderFrameHelper::UpdatePrintSettings(
    blink::WebLocalFrame* frame,
    const blink::WebNode& node,
    const base::DictionaryValue& passed_job_settings) {
  const base::DictionaryValue* job_settings = &passed_job_settings;
  base::DictionaryValue modified_job_settings;
  if (job_settings->empty()) {
    print_preview_context_.set_error(PREVIEW_ERROR_BAD_SETTING);
    return false;
  }

  bool source_is_html = !PrintingNodeOrPdfFrame(frame, node);

  if (!source_is_html) {
    modified_job_settings.MergeDictionary(job_settings);
    modified_job_settings.SetBoolean(kSettingHeaderFooterEnabled, false);
    modified_job_settings.SetInteger(kSettingMarginsType, NO_MARGINS);
    job_settings = &modified_job_settings;
  }

  int cookie =
      print_pages_params_ ? print_pages_params_->params.document_cookie : 0;
  PrintMsg_PrintPages_Params settings;
  bool canceled = false;
  Send(new PrintHostMsg_UpdatePrintSettings(routing_id(), cookie, *job_settings,
                                            &settings, &canceled));
  if (canceled) {
    notify_browser_of_print_failure_ = false;
    return false;
  }

  if (!job_settings->GetInteger(kPreviewUIID, &settings.params.preview_ui_id)) {
    NOTREACHED();
    print_preview_context_.set_error(PREVIEW_ERROR_BAD_SETTING);
    return false;
  }

  if (!job_settings->GetInteger(kPreviewRequestID,
                                &settings.params.preview_request_id) ||
      !job_settings->GetBoolean(kIsFirstRequest,
                                &settings.params.is_first_request)) {
    NOTREACHED();
    print_preview_context_.set_error(PREVIEW_ERROR_BAD_SETTING);
    return false;
  }

  settings.params.print_to_pdf = IsPrintToPdfRequested(*job_settings);
  UpdateFrameMarginsCssInfo(*job_settings);
  settings.params.print_scaling_option = GetPrintScalingOption(
      frame, node, source_is_html, *job_settings, settings.params);

  SetPrintPagesParams(settings);

  if (PrintMsg_Print_Params_IsValid(settings.params))
    return true;

  print_preview_context_.set_error(PREVIEW_ERROR_INVALID_PRINTER_SETTINGS);
  return false;
}
