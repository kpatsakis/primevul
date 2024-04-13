void DownloadItemImpl::SetFullPath(const base::FilePath& new_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() new_path = \"" << new_path.value() << "\" "
            << DebugString(true);
  DCHECK(!new_path.empty());

  TRACE_EVENT_INSTANT2("download", "DownloadItemRenamed",
                       TRACE_EVENT_SCOPE_THREAD, "old_filename",
                       destination_info_.current_path.AsUTF8Unsafe(),
                       "new_filename", new_path.AsUTF8Unsafe());

  destination_info_.current_path = new_path;
}
