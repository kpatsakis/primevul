  bool IsShowingAccountChooser() {
    return PasswordsModelDelegateFromWebContents(WebContents())->GetState() ==
           password_manager::ui::CREDENTIAL_REQUEST_STATE;
  }
