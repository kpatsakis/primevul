void BookmarkEventRouter::BookmarkNodeChanged(BookmarkModel* model,
                                              const BookmarkNode* node) {
  scoped_ptr<ListValue> args(new ListValue());
  args->Append(new StringValue(base::Int64ToString(node->id())));

  DictionaryValue* object_args = new DictionaryValue();
  object_args->SetString(keys::kTitleKey, node->GetTitle());
  if (node->is_url())
    object_args->SetString(keys::kUrlKey, node->url().spec());
  args->Append(object_args);

  DispatchEvent(model->profile(), keys::kOnBookmarkChanged, args.Pass());
}
