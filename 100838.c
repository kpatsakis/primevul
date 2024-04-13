  std::string FixResourceId(const std::string& resource_id) {
    if (resource_id == util::kWapiRootDirectoryResourceId)
      return root_folder_id_;
    return drive_service_->GetResourceIdCanonicalizer().Run(resource_id);
  }
