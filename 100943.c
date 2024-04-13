bool BookmarksUpdateFunction::RunImpl() {
  if (!EditBookmarksEnabled())
    return false;

  scoped_ptr<bookmarks::Update::Params> params(
      bookmarks::Update::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int64 id;
  if (!base::StringToInt64(params->id, &id)) {
    error_ = keys::kInvalidIdError;
    return false;
  }

  BookmarkModel* model = BookmarkModelFactory::GetForProfile(profile());

  string16 title;
  bool has_title = false;
  if (params->changes.title.get()) {
    title = UTF8ToUTF16(*params->changes.title);
    has_title = true;
  }

  std::string url_string;
  if (params->changes.url.get())
    url_string = *params->changes.url;
  GURL url(url_string);
  if (!url_string.empty() && !url.is_valid()) {
    error_ = keys::kInvalidUrlError;
    return false;
  }

  const BookmarkNode* node = model->GetNodeByID(id);
  if (!node) {
    error_ = keys::kNoNodeError;
    return false;
  }
  if (model->is_permanent_node(node)) {
    error_ = keys::kModifySpecialError;
    return false;
  }
  if (has_title)
    model->SetTitle(node, title);
  if (!url.is_empty())
    model->SetURL(node, url);

  scoped_ptr<BookmarkTreeNode> tree_node(
      bookmark_api_helpers::GetBookmarkTreeNode(node, false, false));
  results_ = bookmarks::Update::Results::Create(*tree_node);
  return true;
}
