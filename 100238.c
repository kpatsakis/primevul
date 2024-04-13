    virtual void performTask(ScriptExecutionContext* context)
    {
        ASSERT_UNUSED(context, context->isDocument());
        m_document->dispatchWindowEvent(Event::create(eventNames().hashchangeEvent, false, false));
    }
