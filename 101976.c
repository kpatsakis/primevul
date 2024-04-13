void FileSystemManagerImpl::GetPlatformPathOnFileThread(
    const GURL& path,
    int process_id,
    storage::FileSystemContext* context,
    base::WeakPtr<FileSystemManagerImpl> file_system_manager,
    GetPlatformPathCallback callback) {
  DCHECK(context->default_file_task_runner()->RunsTasksInCurrentSequence());

  SyncGetPlatformPath(
      context, process_id, path,
      base::BindOnce(
          [](base::WeakPtr<FileSystemManagerImpl> file_system_manager,
             GetPlatformPathCallback callback,
             const base::FilePath& platform_path) {
            base::PostTaskWithTraits(
                FROM_HERE, {BrowserThread::IO},
                base::BindOnce(&FileSystemManagerImpl::DidGetPlatformPath,
                               std::move(file_system_manager),
                               std::move(callback), platform_path));
          },
          std::move(file_system_manager), std::move(callback)));
}
