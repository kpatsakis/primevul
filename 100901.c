void SavePackage::OnPathPicked(
    const FilePath& final_name,
    SavePageType type,
    const SavePackageDownloadCreatedCallback& download_created_callback) {
  saved_main_file_path_ = final_name;
  net::GenerateSafeFileName(web_contents()->GetContentsMimeType(), false,
                            &saved_main_file_path_);

  saved_main_directory_path_ = saved_main_file_path_.DirName();
  save_type_ = type;
  if (save_type_ == SAVE_PAGE_TYPE_AS_COMPLETE_HTML) {
    saved_main_directory_path_ = saved_main_directory_path_.Append(
        saved_main_file_path_.RemoveExtension().BaseName().value() +
        FILE_PATH_LITERAL("_files"));
  }

  Init(download_created_callback);
}
