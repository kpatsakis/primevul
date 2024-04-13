bool BookmarksRemoveFunction::RunImpl() {
  if (!EditBookmarksEnabled())
    return false;

  scoped_ptr<bookmarks::Remove::Params> params(
      bookmarks::Remove::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  int64 id;
  if (!base::StringToInt64(params->id, &id)) {
    error_ = keys::kInvalidIdError;
    return false;
  }

  bool recursive = false;
  if (name() == BookmarksRemoveTreeFunction::function_name())
    recursive = true;

  BookmarkModel* model = BookmarkModelFactory::GetForProfile(profile());
  if (!bookmark_api_helpers::RemoveNode(model, id, recursive, &error_))
    return false;

  return true;
}
