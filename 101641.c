  SetIsInertMessageFilter()
      : content::BrowserMessageFilter(FrameMsgStart),
        msg_received_(false) {}
