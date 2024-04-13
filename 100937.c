void BookmarkEventRouter::BookmarkNodeChildrenReordered(
    BookmarkModel* model,
    const BookmarkNode* node) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(new StringValue(base::Int64ToString(node->id())));
  int childCount = node->child_count();
  ListValue* children = new ListValue();
  for (int i = 0; i < childCount; ++i) {
    const BookmarkNode* child = node->GetChild(i);
    Value* child_id = new StringValue(base::Int64ToString(child->id()));
    children->Append(child_id);
  }
  DictionaryValue* reorder_info = new DictionaryValue();
  reorder_info->Set(keys::kChildIdsKey, children);
  args->Append(reorder_info);

  DispatchEvent(model->profile(), keys::kOnBookmarkChildrenReordered,
                args.Pass());
}
