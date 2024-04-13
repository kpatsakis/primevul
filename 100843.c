void V8DOMWindow::locationAttrSetterCustom(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    DOMWindow* imp = V8DOMWindow::toNative(info.Holder());

    DOMWindow* active = activeDOMWindow();
    if (!active)
      return;

    DOMWindow* first = firstDOMWindow();
    if (!first)
      return;

    if (Location* location = imp->location())
        location->setHref(toWebCoreString(value), active, first);
}
