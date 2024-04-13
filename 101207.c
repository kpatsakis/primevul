NavigatorServiceWorker::NavigatorServiceWorker(Navigator& navigator)
    : DOMWindowProperty(navigator.frame())
{
}
