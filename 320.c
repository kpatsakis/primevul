void AddUninstallShortcutWorkItems(const InstallerState& installer_state,
                                   const FilePath& setup_path,
                                   const Version& new_version,
                                   WorkItemList* install_list,
                                   const Product& product) {
  HKEY reg_root = installer_state.root_key();
  BrowserDistribution* browser_dist = product.distribution();
  DCHECK(browser_dist);

  FilePath install_path(installer_state.target_path());
  FilePath installer_path(installer_state.GetInstallerDirectory(new_version));
  installer_path = installer_path.Append(setup_path.BaseName());

  CommandLine uninstall_arguments(CommandLine::NO_PROGRAM);
  AppendUninstallCommandLineFlags(installer_state, product,
                                  &uninstall_arguments);

  if (product.is_chrome() &&
      installer_state.operation() != InstallerState::UNINSTALL) {
    const Products& products = installer_state.products();
    for (size_t i = 0; i < products.size(); ++i) {
      const Product& p = *products[i];
      if (!p.is_chrome() && !p.ShouldCreateUninstallEntry())
        p.AppendUninstallFlags(&uninstall_arguments);
    }
  }

  std::wstring update_state_key(browser_dist->GetStateKey());
  install_list->AddCreateRegKeyWorkItem(reg_root, update_state_key);
  install_list->AddSetRegValueWorkItem(reg_root, update_state_key,
      installer::kUninstallStringField, installer_path.value(), true);
  install_list->AddSetRegValueWorkItem(reg_root, update_state_key,
      installer::kUninstallArgumentsField,
      uninstall_arguments.command_line_string(), true);

  if (!installer_state.is_msi() && product.ShouldCreateUninstallEntry()) {
    CommandLine quoted_uninstall_cmd(installer_path);
    DCHECK_EQ(quoted_uninstall_cmd.command_line_string()[0], '"');
    quoted_uninstall_cmd.AppendArguments(uninstall_arguments, false);

    std::wstring uninstall_reg = browser_dist->GetUninstallRegPath();
    install_list->AddCreateRegKeyWorkItem(reg_root, uninstall_reg);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
        installer::kUninstallDisplayNameField,
        browser_dist->GetAppShortCutName(), true);
    install_list->AddSetRegValueWorkItem(reg_root,
        uninstall_reg, installer::kUninstallStringField,
        quoted_uninstall_cmd.command_line_string(), true);
    install_list->AddSetRegValueWorkItem(reg_root,
                                         uninstall_reg,
                                         L"InstallLocation",
                                         install_path.value(),
                                         true);

    FilePath chrome_icon(install_path.Append(installer::kChromeExe));
    ShellUtil::GetChromeIcon(product.distribution(), chrome_icon.value());
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"DisplayIcon", chrome_icon.value(),
                                         true);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"NoModify", static_cast<DWORD>(1),
                                         true);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"NoRepair", static_cast<DWORD>(1),
                                         true);

    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"Publisher",
                                         browser_dist->GetPublisherName(),
                                         true);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"Version",
                                         UTF8ToWide(new_version.GetString()),
                                         true);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"DisplayVersion",
                                         UTF8ToWide(new_version.GetString()),
                                         true);
    install_list->AddSetRegValueWorkItem(reg_root, uninstall_reg,
                                         L"InstallDate",
                                         InstallUtil::GetCurrentDate(),
                                         false);
  }
}
