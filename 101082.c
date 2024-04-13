void ThreadWatcherList::ParseCommandLine(
    const base::CommandLine& command_line,
    uint32* unresponsive_threshold,
    CrashOnHangThreadMap* crash_on_hang_threads) {
  *unresponsive_threshold = kUnresponsiveCount;

  version_info::Channel channel = chrome::GetChannel();
  if (channel == version_info::Channel::STABLE) {
    *unresponsive_threshold *= 4;
  } else if (channel == version_info::Channel::BETA) {
    *unresponsive_threshold *= 2;
  }

#if defined(OS_WIN)
  if (base::win::GetVersion() <= base::win::VERSION_XP)
    *unresponsive_threshold *= 2;
#endif

  uint32 crash_seconds = *unresponsive_threshold * kUnresponsiveSeconds;
  std::string crash_on_hang_thread_names;
  if (command_line.HasSwitch(switches::kCrashOnHangThreads)) {
    crash_on_hang_thread_names =
        command_line.GetSwitchValueASCII(switches::kCrashOnHangThreads);
  } else if (channel != version_info::Channel::STABLE) {
    crash_on_hang_thread_names = base::StringPrintf(
        "UI:%d:%d,IO:%d:%d,FILE:%d:%d",
        kLiveThreadsThreshold, crash_seconds,
        kLiveThreadsThreshold, crash_seconds,
        kLiveThreadsThreshold, crash_seconds * 5);
  }

  ParseCommandLineCrashOnHangThreads(crash_on_hang_thread_names,
                                     kLiveThreadsThreshold,
                                     crash_seconds,
                                     crash_on_hang_threads);
}
