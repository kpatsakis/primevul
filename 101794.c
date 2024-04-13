inline void ImageLoader::DispatchErrorEvent() {
  pending_error_event_ = PostCancellableTask(
      *GetElement()->GetDocument().GetTaskRunner(TaskType::kDOMManipulation),
      FROM_HERE,
      WTF::Bind(&ImageLoader::DispatchPendingErrorEvent, WrapPersistent(this),
                WTF::Passed(IncrementLoadEventDelayCount::Create(
                    GetElement()->GetDocument()))));
}
