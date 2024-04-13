ExtensionServiceBackend::ExtensionServiceBackend(
    base::WeakPtr<ExtensionService> frontend,
    const FilePath& install_directory)
        : frontend_(frontend),
          install_directory_(install_directory) {
  CHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
}
