bool PrintRenderFrameHelper::OnMessageReceived(const IPC::Message& message) {
  ++ipc_nesting_level_;

  bool handled = true;
  IPC_BEGIN_MESSAGE_MAP(PrintRenderFrameHelper, message)
#if BUILDFLAG(ENABLE_BASIC_PRINTING)
    IPC_MESSAGE_HANDLER(PrintMsg_PrintPages, OnPrintPages)
    IPC_MESSAGE_HANDLER(PrintMsg_PrintForSystemDialog, OnPrintForSystemDialog)
#endif  // BUILDFLAG(ENABLE_BASIC_PRINTING)
#if BUILDFLAG(ENABLE_PRINT_PREVIEW)
    IPC_MESSAGE_HANDLER(PrintMsg_InitiatePrintPreview, OnInitiatePrintPreview)
    IPC_MESSAGE_HANDLER(PrintMsg_PrintPreview, OnPrintPreview)
    IPC_MESSAGE_HANDLER(PrintMsg_PrintingDone, OnPrintingDone)
    IPC_MESSAGE_HANDLER(PrintMsg_ClosePrintPreviewDialog,
                        OnClosePrintPreviewDialog)
#endif  // BUILDFLAG(ENABLE_PRINT_PREVIEW)
    IPC_MESSAGE_HANDLER(PrintMsg_SetPrintingEnabled, OnSetPrintingEnabled)
    IPC_MESSAGE_UNHANDLED(handled = false)
  IPC_END_MESSAGE_MAP()

  --ipc_nesting_level_;
  if (ipc_nesting_level_ == 0 && render_frame_gone_)
    base::ThreadTaskRunnerHandle::Get()->DeleteSoon(FROM_HERE, this);
  return handled;
}
