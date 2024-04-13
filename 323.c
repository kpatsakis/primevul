bool AppendPostInstallTasks(const InstallerState& installer_state,
                            const FilePath& setup_path,
                            const FilePath& new_chrome_exe,
                            const Version* current_version,
                            const Version& new_version,
                            const FilePath& temp_path,
                            WorkItemList* post_install_task_list) {
  DCHECK(post_install_task_list);

  HKEY root = installer_state.root_key();
  const Products& products = installer_state.products();

  {
    scoped_ptr<WorkItemList> in_use_update_work_items(
        WorkItem::CreateConditionalWorkItemList(
            new ConditionRunIfFileExists(new_chrome_exe)));
    in_use_update_work_items->set_log_message("InUseUpdateWorkItemList");

    FilePath installer_path(installer_state.GetInstallerDirectory(new_version)
        .Append(setup_path.BaseName()));

    CommandLine rename(installer_path);
    rename.AppendSwitch(switches::kRenameChromeExe);
    if (installer_state.system_install())
      rename.AppendSwitch(switches::kSystemLevel);

    if (installer_state.verbose_logging())
      rename.AppendSwitch(switches::kVerboseLogging);

    std::wstring version_key;
    for (size_t i = 0; i < products.size(); ++i) {
      BrowserDistribution* dist = products[i]->distribution();
      version_key = dist->GetVersionKey();

      if (current_version != NULL) {
        in_use_update_work_items->AddSetRegValueWorkItem(root, version_key,
            google_update::kRegOldVersionField,
            UTF8ToWide(current_version->GetString()), true);
      }

      CommandLine product_rename_cmd(rename);
      products[i]->AppendRenameFlags(&product_rename_cmd);
      in_use_update_work_items->AddSetRegValueWorkItem(
          root,
          version_key,
          google_update::kRegRenameCmdField,
          product_rename_cmd.command_line_string(),
          true);
    }

    if (current_version != NULL && installer_state.is_multi_install()) {
      BrowserDistribution* dist =
          installer_state.multi_package_binaries_distribution();
      in_use_update_work_items->AddSetRegValueWorkItem(
          root,
          dist->GetVersionKey(),
          google_update::kRegOldVersionField,
          UTF8ToWide(current_version->GetString()),
          true);
    }

    post_install_task_list->AddWorkItem(in_use_update_work_items.release());
  }

  {
    scoped_ptr<WorkItemList> regular_update_work_items(
        WorkItem::CreateConditionalWorkItemList(
            new Not(new ConditionRunIfFileExists(new_chrome_exe))));
    regular_update_work_items->set_log_message("RegularUpdateWorkItemList");

    for (size_t i = 0; i < products.size(); ++i) {
      BrowserDistribution* dist = products[i]->distribution();
      std::wstring version_key(dist->GetVersionKey());
      regular_update_work_items->AddDeleteRegValueWorkItem(root, version_key,
                                            google_update::kRegOldVersionField);
      regular_update_work_items->AddDeleteRegValueWorkItem(root, version_key,
                                            google_update::kRegRenameCmdField);
    }

    post_install_task_list->AddWorkItem(regular_update_work_items.release());
  }

  AddRegisterComDllWorkItemsForPackage(installer_state, current_version,
                                       new_version, post_install_task_list);

  if (installer_state.is_msi()) {
    for (size_t i = 0; i < products.size(); ++i) {
      const Product* product = products[i];
      AddSetMsiMarkerWorkItem(installer_state, product->distribution(), true,
                              post_install_task_list);

      AddDeleteUninstallShortcutsForMSIWorkItems(installer_state, *product,
                                                 temp_path,
                                                 post_install_task_list);
    }
    if (installer_state.is_multi_install()) {
      AddSetMsiMarkerWorkItem(installer_state,
          installer_state.multi_package_binaries_distribution(), true,
          post_install_task_list);
    }
  }

  return true;
}
