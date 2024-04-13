void RenderWidgetHostImpl::DispatchColorProfile() {
#if defined(OS_WIN) || defined(OS_MACOSX) || defined(OS_LINUX)
  static bool image_profiles = base::CommandLine::ForCurrentProcess()->
     HasSwitch(switches::kEnableImageColorProfiles);
  if (!image_profiles)
    return;
#if defined(OS_WIN)
  BrowserThread::PostBlockingPoolTask(
      FROM_HERE,
      base::Bind(&RenderWidgetHostImpl::SendColorProfile,
                 weak_factory_.GetWeakPtr()));
#elif !defined(OS_CHROMEOS) && !defined(OS_IOS) && !defined(OS_ANDROID)
  SendColorProfile();
#endif
#endif
}
