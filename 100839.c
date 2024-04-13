void ChangeListLoader::GetAboutResource(
    const google_apis::AboutResourceCallback& callback) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DCHECK(!callback.is_null());

  if (cached_about_resource_) {
    base::MessageLoopProxy::current()->PostTask(
        FROM_HERE,
        base::Bind(
            callback,
            google_apis::HTTP_NO_CONTENT,
            base::Passed(scoped_ptr<google_apis::AboutResource>(
                new google_apis::AboutResource(*cached_about_resource_)))));
  } else {
    UpdateAboutResource(callback);
  }
}
