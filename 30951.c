FindChildForEvent(SpritePtr pSprite, WindowPtr event)
{
    WindowPtr w = DeepestSpriteWin(pSprite);
    Window child = None;

    /* If the search ends up past the root should the child field be
       set to none or should the value in the argument be passed
       through. It probably doesn't matter since everyone calls
       this function with child == None anyway. */
    while (w) {
        /* If the source window is same as event window, child should be
           none.  Don't bother going all all the way back to the root. */

        if (w == event) {
            child = None;
            break;
        }

        if (w->parent == event) {
            child = w->drawable.id;
            break;
        }
        w = w->parent;
    }
    return child;
}
