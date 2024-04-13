  void ReportResultFromUIThread() {
    CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
    if (error_.empty() && parsed_manifest_.get())
      client_->OnParseSuccess(icon_, parsed_manifest_.release());
    else
      client_->OnParseFailure(parse_error_, error_);
  }
