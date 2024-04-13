    AuthenticatorInsertAndActivateUsbSheetModel(
        AuthenticatorRequestDialogModel* dialog_model)
    : AuthenticatorSheetModelBase(dialog_model),
      other_transports_menu_model_(std::make_unique<OtherTransportsMenuModel>(
          dialog_model,
          AuthenticatorTransport::kUsbHumanInterfaceDevice)) {}
