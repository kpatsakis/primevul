PassRefPtr<WebProcessProxy> WebProcessProxy::create(PassRefPtr<WebContext> context)
{
    return adoptRef(new WebProcessProxy(context));
}
