FilePath ChromeDownloadManagerDelegate::GetIntermediatePath(
    const FilePath& target_path,
    content::DownloadDangerType danger_type) {
  if (danger_type == content::DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS)
    return download_util::GetCrDownloadPath(target_path);

  FilePath::StringType file_name;
  FilePath dir = target_path.DirName();
#if defined(OS_WIN)
  string16 unconfirmed_prefix =
      l10n_util::GetStringUTF16(IDS_DOWNLOAD_UNCONFIRMED_PREFIX);
#else
  std::string unconfirmed_prefix =
      l10n_util::GetStringUTF8(IDS_DOWNLOAD_UNCONFIRMED_PREFIX);
#endif
  base::SStringPrintf(
      &file_name,
      unconfirmed_prefix.append(
          FILE_PATH_LITERAL(" %d.crdownload")).c_str(),
      base::RandInt(0, 1000000));
  return dir.Append(file_name);
}
