void ChangeListLoader::LoadAfterGetLargestChangestamp(
    const DirectoryFetchInfo& directory_fetch_info,
    bool is_initial_load,
    int64 local_changestamp) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  if (is_initial_load && local_changestamp > 0) {
    OnChangeListLoadComplete(FILE_ERROR_OK);

    pending_load_callback_[directory_fetch_info.local_id()].push_back(
        base::Bind(&util::EmptyFileOperationCallback));
    if (!directory_fetch_info.empty()) {
      pending_load_callback_[""].push_back(
          base::Bind(&util::EmptyFileOperationCallback));
    }
  }

  if (directory_fetch_info.empty()) {
    UpdateAboutResource(base::Bind(&ChangeListLoader::LoadAfterGetAboutResource,
                                   weak_ptr_factory_.GetWeakPtr(),
                                   directory_fetch_info,
                                   is_initial_load,
                                   local_changestamp));
  } else {
    GetAboutResource(base::Bind(&ChangeListLoader::LoadAfterGetAboutResource,
                                weak_ptr_factory_.GetWeakPtr(),
                                directory_fetch_info,
                                is_initial_load,
                                local_changestamp));
  }
}
