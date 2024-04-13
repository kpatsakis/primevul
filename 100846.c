v8::Handle<v8::Value> V8DOMWindow::setTimeoutMethodCustom(const v8::Arguments& args)
{
    return WindowSetTimeoutImpl(args, true);
}
