void BookmarkManagerView::OnTreeViewKeyDown(unsigned short virtual_keycode) {
  switch (virtual_keycode) {
    case VK_DELETE: {
      BookmarkNode* node = GetSelectedFolder();
      if (!node || node->GetParent() == GetBookmarkModel()->root_node())
        return;

      BookmarkNode* parent = node->GetParent();
      GetBookmarkModel()->Remove(parent, parent->IndexOfChild(node));
      break;
    }

    default:
      OnCutCopyPaste(KeyCodeToCutCopyPaste(virtual_keycode), false);
      break;
  }
}
