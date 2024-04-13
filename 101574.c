base::string16 AuthenticatorBlePinEntrySheetModel::GetStepTitle() const {
  const auto& authenticator_id = dialog_model()->selected_authenticator_id();
  DCHECK(authenticator_id);
  const auto* ble_authenticator =
      dialog_model()->saved_authenticators().GetAuthenticator(
          *authenticator_id);
  DCHECK(ble_authenticator);
  return l10n_util::GetStringFUTF16(
      IDS_WEBAUTHN_BLE_PIN_ENTRY_TITLE,
      ble_authenticator->authenticator_display_name());
}
