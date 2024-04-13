void ScriptController::clearWindowShell()
{
    double start = currentTime();
    m_windowShell->clearForNavigation();
    for (IsolatedWorldMap::iterator iter = m_isolatedWorlds.begin(); iter != m_isolatedWorlds.end(); ++iter)
        iter->value->clearForNavigation();
    V8GCController::hintForCollectGarbage();
    HistogramSupport::histogramCustomCounts("WebCore.ScriptController.clearWindowShell", (currentTime() - start) * 1000, 0, 10000, 50);
}
