void Browser::FileSelectedWithExtraInfo(const ui::SelectedFileInfo& file_info,
                                        int index,
                                        void* params) {
  profile_->set_last_selected_directory(file_info.file_path.DirName());

  GURL url = net::FilePathToFileURL(file_info.local_path);

#if defined(OS_CHROMEOS)
  const GURL external_url =
      chromeos::CreateExternalFileURLFromPath(profile_, file_info.file_path);
  if (!external_url.is_empty())
    url = external_url;
#endif

  if (url.is_empty())
    return;

  OpenURL(OpenURLParams(url, Referrer(), WindowOpenDisposition::CURRENT_TAB,
                        ui::PAGE_TRANSITION_TYPED, false));
}
