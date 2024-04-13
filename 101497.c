void LogHTMLForm(SavePasswordProgressLogger* logger,
                 SavePasswordProgressLogger::StringID message_id,
                 const WebFormElement& form) {
  logger->LogHTMLForm(message_id, form.GetName().Utf8(),
                      GURL(form.Action().Utf8()));
}
