void OnWebIntentDispatchCompleted(
    const FilePath& file_path,
    webkit_glue::WebIntentReplyType intent_reply) {
  BrowserThread::PostTask(BrowserThread::FILE, FROM_HERE,
                          base::Bind(&DeleteFile, file_path));
}
