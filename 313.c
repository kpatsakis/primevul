void AddElevationPolicyWorkItems(const InstallationState& original_state,
                                 const InstallerState& installer_state,
                                 const Version& new_version,
                                 WorkItemList* install_list) {
  if (!installer_state.is_multi_install()) {
    VLOG(1) << "Not adding elevation policy for single installs";
    return;
  } else {
    const ProductState* cf_state =
        original_state.GetProductState(installer_state.system_install(),
                                       BrowserDistribution::CHROME_FRAME);
    if (cf_state && !cf_state->is_multi_install()) {
      LOG(WARNING) << "Not adding elevation policy since a single install "
                      "of CF exists";
      return;
    }
  }

  FilePath binary_dir(
      GetChromeInstallPath(installer_state.system_install(),
          BrowserDistribution::GetSpecificDistribution(
              BrowserDistribution::CHROME_BINARIES)));

  struct {
    const wchar_t* sub_key;
    const wchar_t* executable;
    const FilePath exe_dir;
  } low_rights_entries[] = {
    { L"ElevationPolicy\\", kChromeLauncherExe,
      binary_dir.Append(ASCIIToWide(new_version.GetString())) },
    { L"DragDrop\\", chrome::kBrowserProcessExecutableName, binary_dir },
  };

  bool uninstall = (installer_state.operation() == InstallerState::UNINSTALL);
  HKEY root = installer_state.root_key();
  const wchar_t kLowRightsKeyPath[] =
      L"SOFTWARE\\Microsoft\\Internet Explorer\\Low Rights\\";
  std::wstring key_path(kLowRightsKeyPath);

  wchar_t cf_classid[64] = {0};
  StringFromGUID2(__uuidof(ChromeFrame), cf_classid, arraysize(cf_classid));

  for (size_t i = 0; i < arraysize(low_rights_entries); ++i) {
    key_path.append(low_rights_entries[i].sub_key).append(cf_classid);
    if (uninstall) {
      install_list->AddDeleteRegKeyWorkItem(root, key_path);
    } else {
      install_list->AddCreateRegKeyWorkItem(root, key_path);
      install_list->AddSetRegValueWorkItem(root, key_path, L"Policy",
          static_cast<DWORD>(3), true);
      install_list->AddSetRegValueWorkItem(root, key_path, L"AppName",
          low_rights_entries[i].executable, true);
      install_list->AddSetRegValueWorkItem(root, key_path, L"AppPath",
          low_rights_entries[i].exe_dir.value(), true);
    }
    key_path.resize(arraysize(kLowRightsKeyPath) - 1);
  }
}
