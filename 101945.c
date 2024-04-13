bool GetCollectStatsConsent() {
  bool enabled = true;

  if (ReportingIsEnforcedByPolicy(&enabled))
    return enabled;

  const bool system_install = IsSystemInstall();

  DWORD out_value = 0;

  if (system_install &&
      nt::QueryRegValueDWORD(
          nt::HKLM, nt::WOW6432,
          InstallDetails::Get().GetClientStateMediumKeyPath().c_str(),
          kRegValueUsageStats, &out_value)) {
    return (out_value == 1);
  }

  return (nt::QueryRegValueDWORD(
              system_install ? nt::HKLM : nt::HKCU, nt::WOW6432,
              InstallDetails::Get().GetClientStateKeyPath().c_str(),
              kRegValueUsageStats, &out_value) &&
          out_value == 1);
}
