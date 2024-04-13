void BookmarksFunction::Run() {
  BookmarkModel* model = BookmarkModelFactory::GetForProfile(profile());
  if (!model->IsLoaded()) {
    model->AddObserver(this);
    AddRef();  // Balanced in Loaded().
    return;
  }

  bool success = RunImpl();
  if (success) {
    content::NotificationService::current()->Notify(
        chrome::NOTIFICATION_EXTENSION_BOOKMARKS_API_INVOKED,
        content::Source<const Extension>(GetExtension()),
        content::Details<const BookmarksFunction>(this));
  }
  SendResponse(success);
}
