void ConfirmEmailDialogDelegate::OnCanceled() {
  base::ResetAndReturn(&callback_).Run(START_SYNC);
}
