void AddInstallWorkItems(const InstallationState& original_state,
                         const InstallerState& installer_state,
                         const FilePath& setup_path,
                         const FilePath& archive_path,
                         const FilePath& src_path,
                         const FilePath& temp_path,
                         const Version& new_version,
                         scoped_ptr<Version>* current_version,
                         WorkItemList* install_list) {
  DCHECK(install_list);

  const FilePath& target_path = installer_state.target_path();

  install_list->AddCreateDirWorkItem(temp_path);
  install_list->AddCreateDirWorkItem(target_path);

  if (current_version != NULL && current_version->get() != NULL) {
    FilePath old_installer_dir(
        installer_state.GetInstallerDirectory(**current_version));
    FilePath old_archive(old_installer_dir.Append(archive_path.BaseName()));
    install_list->AddDeleteTreeWorkItem(old_archive, temp_path)
        ->set_ignore_failure(true);
  }

  FilePath new_chrome_exe(target_path.Append(installer::kChromeNewExe));

  install_list->AddDeleteTreeWorkItem(new_chrome_exe, temp_path);
  install_list->AddCopyTreeWorkItem(
      src_path.Append(installer::kChromeExe).value(),
      target_path.Append(installer::kChromeExe).value(),
      temp_path.value(), WorkItem::NEW_NAME_IF_IN_USE, new_chrome_exe.value());

  if (base::win::OSInfo::GetInstance()->wow64_status() !=
      base::win::OSInfo::WOW64_DISABLED) {
    install_list->AddMoveTreeWorkItem(
        src_path.Append(installer::kWowHelperExe).value(),
        target_path.Append(installer::kWowHelperExe).value(),
        temp_path.value(),
        WorkItem::ALWAYS_MOVE);
  }

  bool check_for_duplicates =
      (current_version != NULL && current_version->get() != NULL &&
       current_version->get()->Equals(new_version));
  install_list->AddMoveTreeWorkItem(
      src_path.AppendASCII(new_version.GetString()).value(),
      target_path.AppendASCII(new_version.GetString()).value(),
      temp_path.value(),
      check_for_duplicates ? WorkItem::CHECK_DUPLICATES :
                             WorkItem::ALWAYS_MOVE);

  install_list->AddCopyTreeWorkItem(
      src_path.Append(installer::kDictionaries).value(),
      target_path.Append(installer::kDictionaries).value(),
      temp_path.value(), WorkItem::IF_NOT_PRESENT);

  install_list->AddDeleteTreeWorkItem(
      target_path.Append(installer::kChromeOldExe), temp_path);

  AddInstallerCopyTasks(installer_state, setup_path, archive_path, temp_path,
                        new_version, install_list);

  const HKEY root = installer_state.root_key();

  const Products& products = installer_state.products();
  for (size_t i = 0; i < products.size(); ++i) {
    const Product* product = products[i];

    AddUninstallShortcutWorkItems(installer_state, setup_path, new_version,
                                  install_list, *product);

    AddVersionKeyWorkItems(root, product->distribution(), new_version,
                           install_list);
  }

  if (installer_state.is_multi_install()) {
    AddVersionKeyWorkItems(root,
        installer_state.multi_package_binaries_distribution(), new_version,
        install_list);
  }

  AddProductSpecificWorkItems(original_state, installer_state, setup_path,
                              new_version, install_list);

  AddGoogleUpdateWorkItems(original_state, installer_state, install_list);

  AddQuickEnableWorkItems(installer_state, original_state, &setup_path,
                          &new_version, install_list);

  AppendPostInstallTasks(installer_state,
                         setup_path,
                         new_chrome_exe,
                         current_version->get(),
                         new_version,
                         temp_path,
                         install_list);
}
