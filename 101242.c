void OutOfProcessInstance::DidOpen(int32_t result) {
  if (result == PP_OK) {
    if (!engine_->HandleDocumentLoad(embed_loader_)) {
      document_load_state_ = LOAD_STATE_LOADING;
      DocumentLoadFailed();
    }
  } else if (result != PP_ERROR_ABORTED) {  // Can happen in tests.
    NOTREACHED();
    DocumentLoadFailed();
  }

  if (engine_->IsProgressiveLoad()) {
    pp::VarDictionary message;
    message.Set(kType, kJSCancelStreamUrlType);
    PostMessage(message);
  }
}
