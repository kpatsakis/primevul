void GoogleChromeDistribution::UpdateInstallStatus(bool system_install,
    installer::ArchiveType archive_type,
    installer::InstallStatus install_status) {
  GoogleUpdateSettings::UpdateInstallStatus(
      system_install, archive_type,
      InstallUtil::GetInstallReturnCode(install_status),
      install_static::GetAppGuid());
}
