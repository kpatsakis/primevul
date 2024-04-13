void WebContentsImpl::OnOpenDateTimeDialog(
    const ViewHostMsg_DateTimeDialogValue_Params& value) {
  date_time_chooser_->ShowDialog(ContentViewCore::FromWebContents(this),
                                 GetRenderViewHost(),
                                 value.dialog_type,
                                 value.year,
                                 value.month,
                                 value.day,
                                 value.hour,
                                 value.minute,
                                 value.second,
                                 value.milli,
                                 value.week,
                                 value.minimum,
                                 value.maximum,
                                 value.step);
}
