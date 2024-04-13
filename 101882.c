 ProfilingProcessHost* ProfilingProcessHost::Start(
     content::ServiceManagerConnection* connection,
    Mode mode) {
  DCHECK(content::BrowserThread::CurrentlyOn(content::BrowserThread::UI));
  CHECK(!has_started_);
  has_started_ = true;
  ProfilingProcessHost* host = GetInstance();
  host->SetMode(mode);
  host->Register();
  host->MakeConnector(connection);
  host->LaunchAsService();
  host->ConfigureBackgroundProfilingTriggers();
  host->metrics_timer_.Start(
      FROM_HERE, base::TimeDelta::FromHours(24),
      base::Bind(&ProfilingProcessHost::ReportMetrics, base::Unretained(host)));

  return host;
}
