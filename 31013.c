XItoCoreType(int xitype)
{
    int coretype = 0;

    if (xitype == DeviceMotionNotify)
        coretype = MotionNotify;
    else if (xitype == DeviceButtonPress)
        coretype = ButtonPress;
    else if (xitype == DeviceButtonRelease)
        coretype = ButtonRelease;
    else if (xitype == DeviceKeyPress)
        coretype = KeyPress;
    else if (xitype == DeviceKeyRelease)
        coretype = KeyRelease;

    return coretype;
}
