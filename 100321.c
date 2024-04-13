void MenuGtk::BuildMenuFromDelegate() {
  for (int i = 1; i <= delegate_->GetItemCount(); ++i) {
    GtkWidget* menu_item = NULL;

    if (delegate_->IsItemSeparator(i)) {
      menu_item = gtk_separator_menu_item_new();
    } else if (delegate_->HasIcon(i)) {
      const SkBitmap* icon = delegate_->GetIcon(i);
      menu_item = BuildMenuItemWithImage(delegate_->GetLabel(i), *icon);
    } else {
      menu_item = gtk_menu_item_new_with_label(delegate_->GetLabel(i).c_str());
    }

    AppendMenuItem(i, menu_item);
  }
}
