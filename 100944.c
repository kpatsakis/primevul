bool BookmarksExportFunction::RunImpl() {
  SelectFile(ui::SelectFileDialog::SELECT_SAVEAS_FILE);
  return true;
}
