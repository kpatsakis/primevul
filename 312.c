void AddChromeFrameWorkItems(const InstallationState& original_state,
                             const InstallerState& installer_state,
                             const FilePath& setup_path,
                             const Version& new_version,
                             const Product& product,
                             WorkItemList* list) {
  DCHECK(product.is_chrome_frame());
  if (!installer_state.is_multi_install()) {
    VLOG(1) << "Not adding GCF specific work items for single install.";
    return;
  }

  std::wstring version_key(product.distribution()->GetVersionKey());
  bool ready_mode = product.HasOption(kOptionReadyMode);
  HKEY root = installer_state.root_key();
  const bool is_install =
      (installer_state.operation() != InstallerState::UNINSTALL);
  bool update_chrome_uninstall_command = false;
  BrowserDistribution* dist =
      installer_state.multi_package_binaries_distribution();
  if (ready_mode) {
    list->AddCreateRegKeyWorkItem(root, dist->GetStateKey());
    list->AddSetRegValueWorkItem(root, dist->GetStateKey(),
        kChromeFrameReadyModeField,
        static_cast<int64>(is_install ? 1 : 0),  // The value we want to set.
        is_install ? false : true);  // Overwrite existing value.
    if (is_install) {
      FilePath installer_path(installer_state
          .GetInstallerDirectory(new_version).Append(setup_path.BaseName()));

      CommandLine basic_cl(installer_path);
      basic_cl.AppendSwitch(switches::kChromeFrame);
      basic_cl.AppendSwitch(switches::kMultiInstall);

      if (installer_state.system_install())
        basic_cl.AppendSwitch(switches::kSystemLevel);

      CommandLine temp_opt_out(basic_cl);
      temp_opt_out.AppendSwitch(switches::kChromeFrameReadyModeTempOptOut);

      CommandLine end_temp_opt_out(basic_cl);
      end_temp_opt_out.AppendSwitch(
          switches::kChromeFrameReadyModeEndTempOptOut);

      CommandLine opt_out(installer_path);
      AppendUninstallCommandLineFlags(installer_state, product, &opt_out);
      opt_out.AppendSwitch(switches::kForceUninstall);

      CommandLine opt_in(basic_cl);
      opt_in.AppendSwitch(switches::kChromeFrameReadyModeOptIn);

      list->AddSetRegValueWorkItem(root, version_key,
                                   google_update::kRegCFTempOptOutCmdField,
                                   temp_opt_out.command_line_string(), true);
      list->AddSetRegValueWorkItem(root, version_key,
                                   google_update::kRegCFEndTempOptOutCmdField,
                                   end_temp_opt_out.command_line_string(),
                                   true);
      list->AddSetRegValueWorkItem(root, version_key,
                                   google_update::kRegCFOptOutCmdField,
                                   opt_out.command_line_string(), true);
      list->AddSetRegValueWorkItem(root, version_key,
                                   google_update::kRegCFOptInCmdField,
                                   opt_in.command_line_string(), true);
    } else {
      update_chrome_uninstall_command =
          (installer_state.FindProduct(BrowserDistribution::CHROME_BROWSER) ==
           NULL);
    }
  } else {

    list->AddDeleteRegValueWorkItem(root, dist->GetStateKey(),
        kChromeFrameReadyModeField);

    const Product* chrome =
        installer_state.FindProduct(BrowserDistribution::CHROME_BROWSER);
    if (chrome) {
    } else {
      const ProductState* chrome_state = original_state.GetProductState(
          installer_state.system_install(),
          BrowserDistribution::CHROME_BROWSER);
      update_chrome_uninstall_command =
          (chrome_state != NULL) && chrome_state->is_multi_install();
    }
  }

  if (!ready_mode || !is_install) {
    list->AddDeleteRegValueWorkItem(root, version_key,
                                    google_update::kRegCFTempOptOutCmdField);
    list->AddDeleteRegValueWorkItem(root, version_key,
                                    google_update::kRegCFEndTempOptOutCmdField);
    list->AddDeleteRegValueWorkItem(root, version_key,
                                    google_update::kRegCFOptOutCmdField);
    list->AddDeleteRegValueWorkItem(root, version_key,
                                    google_update::kRegCFOptInCmdField);
  }

  if (update_chrome_uninstall_command) {
    const ProductState* chrome_state = original_state.GetProductState(
        installer_state.system_install(),
        BrowserDistribution::CHROME_BROWSER);
    if (chrome_state != NULL) {
      DCHECK(chrome_state->is_multi_install());
      Product chrome(BrowserDistribution::GetSpecificDistribution(
                         BrowserDistribution::CHROME_BROWSER));
      chrome.InitializeFromUninstallCommand(chrome_state->uninstall_command());
      AddUninstallShortcutWorkItems(installer_state, setup_path,
                                    chrome_state->version(), list, chrome);
    } else {
      NOTREACHED() << "What happened to Chrome?";
    }
  }
}
