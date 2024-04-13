void WebProcessProxy::sendDidGetPlugins(uint64_t requestID, PassOwnPtr<Vector<PluginInfo> > pluginInfos)
{
    ASSERT(isMainThread());

    OwnPtr<Vector<PluginInfo> > plugins(pluginInfos);

#if PLATFORM(MAC)
    if (!m_context->omitPDFSupport()) {
#if ENABLE(PDFKIT_PLUGIN)
        plugins->append(PDFPlugin::pluginInfo());
#endif
        plugins->append(SimplePDFPlugin::pluginInfo());
    }
#endif

    send(Messages::WebProcess::DidGetPlugins(requestID, *plugins), 0);
}
