XNewModifiermap(int keyspermodifier)
{
    XModifierKeymap *res = Xmalloc((sizeof (XModifierKeymap)));
    if (res) {
	res->max_keypermod = keyspermodifier;
	res->modifiermap = (keyspermodifier > 0 ?
			    Xmalloc(8 * keyspermodifier)
			    : (KeyCode *) NULL);
	if (keyspermodifier && (res->modifiermap == NULL)) {
	    Xfree(res);
	    return (XModifierKeymap *) NULL;
	}
    }
    return (res);
}
