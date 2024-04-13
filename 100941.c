bool BookmarksGetRecentFunction::RunImpl() {
  scoped_ptr<bookmarks::GetSubTree::Params> params(
      bookmarks::GetSubTree::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int64 id;
  if (!GetBookmarkIdAsInt64(params->id, &id))
    return false;

  const BookmarkNode* node =
      BookmarkModelFactory::GetForProfile(profile())->GetNodeByID(id);
  if (!node) {
    error_ = keys::kNoNodeError;
    return false;
  }

  std::vector<linked_ptr<BookmarkTreeNode> > nodes;
  bookmark_api_helpers::AddNode(node, &nodes, true);
  results_ = bookmarks::GetSubTree::Results::Create(nodes);
  return true;
}
