void WebProcessProxy::getLaunchOptions(ProcessLauncher::LaunchOptions& launchOptions)
{
    launchOptions.processType = ProcessLauncher::WebProcess;
    platformGetLaunchOptions(launchOptions);
}
