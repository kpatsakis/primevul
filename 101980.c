std::unique_ptr<service_manager::Service> StartDownloadManager() {
  return DownloadManagerService::GetInstance()
      ->CreateServiceManagerServiceInstance();
}
