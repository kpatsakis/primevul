v8::Handle<v8::Value> V8DOMWindow::removeEventListenerMethodCustom(const v8::Arguments& args)
{
    String eventType = toWebCoreString(args[0]);
    bool useCapture = args[2]->BooleanValue();

    DOMWindow* imp = V8DOMWindow::toNative(args.Holder());

    if (!BindingSecurity::shouldAllowAccessToFrame(imp->frame()))
        return v8::Undefined();

    Document* doc = imp->document();

    if (!doc)
        return v8::Undefined();

    if (!imp->frame())
        return v8::Undefined();

    RefPtr<EventListener> listener = V8EventListenerList::getEventListener(args[1], false, ListenerFindOnly);

    if (listener) {
        imp->removeEventListener(eventType, listener.get(), useCapture);
        removeHiddenDependency(args.Holder(), args[1], eventListenerCacheIndex, args.GetIsolate());
    }

    return v8::Undefined();
}
