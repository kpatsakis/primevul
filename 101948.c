std::wstring GetUninstallRegistryPath() {
  std::wstring result(
      L"Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
  if (*kCompanyPathName)
    result.append(kCompanyPathName).append(1, L' ');
  result.append(kProductPathName, kProductPathNameLength);
  return result.append(InstallDetails::Get().mode().install_suffix);
}
