void ProfilingProcessHost::RequestProcessReport(std::string trigger_name) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  if (!connector_) {
    DLOG(ERROR)
        << "Requesting process dump when profiling process hasn't started.";
    return;
  }

  bool result = content::TracingController::GetInstance()->StartTracing(
      GetBackgroundTracingConfig(), base::Closure());
  if (!result)
    return;

  auto finish_sink_callback = base::Bind(
      [](std::string trigger_name,
         std::unique_ptr<const base::DictionaryValue> metadata,
         base::RefCountedString* in) {
        std::string result;
        result.swap(in->data());
        content::BrowserThread::GetTaskRunnerForThread(
            content::BrowserThread::UI)
            ->PostTask(FROM_HERE, base::BindOnce(&UploadTraceToCrashServer,
                                                 std::move(result),
                                                 std::move(trigger_name)));
      },
      std::move(trigger_name));

  scoped_refptr<content::TracingController::TraceDataEndpoint> sink =
      content::TracingController::CreateStringEndpoint(
          std::move(finish_sink_callback));
  base::OnceClosure stop_tracing_closure = base::BindOnce(
      base::IgnoreResult<bool (content::TracingController::*)(  // NOLINT
          const scoped_refptr<content::TracingController::TraceDataEndpoint>&)>(
          &content::TracingController::StopTracing),
      base::Unretained(content::TracingController::GetInstance()), sink);

  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE, std::move(stop_tracing_closure),
      base::TimeDelta::FromSeconds(10));
}
