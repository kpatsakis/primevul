void DocumentThreadableLoader::reportResponseReceived(unsigned long identifier, const ResourceResponse& response)
{
    LocalFrame* frame = document().frame();
    ASSERT(frame);
    if (!frame)
        return;
    DocumentLoader* loader = frame->loader().documentLoader();
    TRACE_EVENT_INSTANT1("devtools.timeline", "ResourceReceiveResponse", TRACE_EVENT_SCOPE_THREAD, "data", InspectorReceiveResponseEvent::data(identifier, frame, response));
    InspectorInstrumentation::didReceiveResourceResponse(frame, identifier, loader, response, resource() ? resource()->loader() : 0);
    frame->console().reportResourceResponseReceived(loader, identifier, response);
}
