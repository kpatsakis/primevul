void DevToolsWindow::FileSystemsLoaded(
    const std::vector<DevToolsFileHelper::FileSystem>& file_systems) {
  ListValue file_systems_value;
  for (size_t i = 0; i < file_systems.size(); ++i)
    file_systems_value.Append(CreateFileSystemValue(file_systems[i]));
  CallClientFunction("InspectorFrontendAPI.fileSystemsLoaded",
                     &file_systems_value, NULL, NULL);
}
