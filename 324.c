void AppendUninstallCommandLineFlags(const InstallerState& installer_state,
                                     const Product& product,
                                     CommandLine* uninstall_cmd) {
  DCHECK(uninstall_cmd);

  uninstall_cmd->AppendSwitch(installer::switches::kUninstall);

  product.AppendUninstallFlags(uninstall_cmd);
  if (installer_state.is_msi()) {
    uninstall_cmd->AppendSwitch(installer::switches::kMsi);
    if (product.is_chrome_frame()) {
      uninstall_cmd->AppendSwitch(installer::switches::kDeleteProfile);
    }
  }
  if (installer_state.system_install())
    uninstall_cmd->AppendSwitch(installer::switches::kSystemLevel);
  if (installer_state.verbose_logging())
    uninstall_cmd->AppendSwitch(installer::switches::kVerboseLogging);
}
