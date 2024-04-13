gfx::ImageSkia GetBookmarkFolderIcon(SkColor text_color) {
#if defined(OS_WIN)
  return *ui::ResourceBundle::GetSharedInstance().GetImageSkiaNamed(
      IDR_BOOKMARK_BAR_FOLDER);
#else
  return GetFolderIcon(gfx::VectorIconId::FOLDER, text_color);
#endif
}
