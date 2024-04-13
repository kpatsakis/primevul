void DevToolsWindow::SaveToFile(const std::string& url,
                                const std::string& content,
                                bool save_as) {
  file_helper_->Save(url, content, save_as,
                     base::Bind(&DevToolsWindow::FileSavedAs,
                                weak_factory_.GetWeakPtr(), url));
}
