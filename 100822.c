void DevToolsWindow::RemoveFileSystem(const std::string& file_system_path) {
  CHECK(web_contents_->GetURL().SchemeIs(chrome::kChromeDevToolsScheme));
  file_helper_->RemoveFileSystem(file_system_path);
  StringValue file_system_path_value(file_system_path);
  CallClientFunction("InspectorFrontendAPI.fileSystemRemoved",
                     &file_system_path_value, NULL, NULL);
}
