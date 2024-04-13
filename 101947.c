std::wstring GetLegacyCommandExecuteImplClsid() {
  return InstallDetails::Get().mode().legacy_command_execute_clsid;
}
