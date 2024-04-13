scoped_refptr<WebTaskRunner> Document::GetTaskRunner(TaskType type) {
  if (ContextDocument() && ContextDocument()->GetFrame())
    return ContextDocument()->GetFrame()->FrameScheduler()->GetTaskRunner(type);
  return Platform::Current()->CurrentThread()->GetWebTaskRunner();
}
