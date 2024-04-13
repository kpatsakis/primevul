void ProfilingProcessHost::LaunchAsService() {
  if (!content::BrowserThread::CurrentlyOn(content::BrowserThread::IO)) {
    content::BrowserThread::GetTaskRunnerForThread(content::BrowserThread::IO)
        ->PostTask(FROM_HERE,
                   base::BindOnce(&ProfilingProcessHost::LaunchAsService,
                                  base::Unretained(this)));
    return;
  }

  base::trace_event::MemoryDumpManager::GetInstance()->RegisterDumpProvider(
      this, "OutOfProcessHeapProfilingDumpProvider",
      content::BrowserThread::GetTaskRunnerForThread(
          content::BrowserThread::IO));

  connector_->BindInterface(mojom::kServiceName, &profiling_service_);

  if (ShouldProfileProcessType(content::ProcessType::PROCESS_TYPE_BROWSER)) {
    ProfilingClientBinder client(connector_.get());
    AddClientToProfilingService(client.take(), base::Process::Current().Pid(),
                                profiling::mojom::ProcessType::BROWSER);
  }
}
