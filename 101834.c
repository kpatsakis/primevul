void RenderProcessHostImpl::OnProcessLaunched() {
  if (deleting_soon_)
    return;

  if (child_process_launcher_) {
    DCHECK(child_process_launcher_->GetProcess().IsValid());
    DCHECK_EQ(kLaunchingProcessIsBackgrounded, priority_.background);

    channel_->Unpause(false /* flush */);

    if (child_connection_) {
      child_connection_->SetProcessHandle(
          child_process_launcher_->GetProcess().Handle());
    }

#if defined(OS_MACOSX)
    priority_.background =
        child_process_launcher_->GetProcess().IsProcessBackgrounded(
            MachBroker::GetInstance());
#elif defined(OS_ANDROID)
    DCHECK_EQ(kLaunchingProcessIsBackgrounded, priority_.background);
#else
    priority_.background =
        child_process_launcher_->GetProcess().IsProcessBackgrounded();
#endif  // defined(OS_MACOSX)

#if defined(OS_ANDROID)
    UpdateProcessPriority();
#endif

    CreateSharedRendererHistogramAllocator();
  }

  NotificationService::current()->Notify(NOTIFICATION_RENDERER_PROCESS_CREATED,
                                         Source<RenderProcessHost>(this),
                                         NotificationService::NoDetails());

  if (child_process_launcher_)
    channel_->Flush();

  if (IsReady()) {
    DCHECK(!sent_render_process_ready_);
    sent_render_process_ready_ = true;
    for (auto& observer : observers_)
      observer.RenderProcessReady(this);
  }

  GetProcessResourceCoordinator()->SetLaunchTime(base::Time::Now());
  GetProcessResourceCoordinator()->SetPID(base::GetProcId(GetHandle()));

#if BUILDFLAG(ENABLE_WEBRTC)
  WebRTCInternals* webrtc_internals = WebRTCInternals::GetInstance();
  if (webrtc_internals->IsAudioDebugRecordingsEnabled()) {
    EnableAudioDebugRecordings(
        webrtc_internals->GetAudioDebugRecordingsFilePath());
  }
#endif
}
