PassRefPtrWillBeRawPtr<Touch> Document::createTouch(DOMWindow* window, EventTarget* target, int identifier, double pageX, double pageY, double screenX, double screenY, double radiusX, double radiusY, float rotationAngle, float force) const
{
    if (!std::isfinite(pageX))
        pageX = 0;
    if (!std::isfinite(pageY))
        pageY = 0;
    if (!std::isfinite(screenX))
        screenX = 0;
    if (!std::isfinite(screenY))
        screenY = 0;
    if (!std::isfinite(radiusX))
        radiusX = 0;
    if (!std::isfinite(radiusY))
        radiusY = 0;
    if (!std::isfinite(rotationAngle))
        rotationAngle = 0;
    if (!std::isfinite(force))
        force = 0;

    LocalFrame* frame = window && window->isLocalDOMWindow() ? toLocalDOMWindow(window)->frame() : this->frame();
    return Touch::create(frame, target, identifier, FloatPoint(screenX, screenY), FloatPoint(pageX, pageY), FloatSize(radiusX, radiusY), rotationAngle, force);
}
