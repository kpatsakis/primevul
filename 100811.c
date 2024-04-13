void DevToolsWindow::AppendToFile(const std::string& url,
                                  const std::string& content) {
  file_helper_->Append(url, content,
                       base::Bind(&DevToolsWindow::AppendedTo,
                                  weak_factory_.GetWeakPtr(), url));
}
