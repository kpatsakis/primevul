  explicit ShutdownWatchDogThread(const base::TimeDelta& duration)
      : base::Watchdog(duration, "Shutdown watchdog thread", true) {
  }
