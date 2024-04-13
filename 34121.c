XpmCreateDataFromImage(
    Display		  *display,
    char		***data_return,
    XImage		  *image,
    XImage		  *shapeimage,
    XpmAttributes	  *attributes)
{
    XpmImage xpmimage;
    XpmInfo info;
    int ErrorStatus;

    /* initialize return value */
    if (data_return)
	*data_return = NULL;

    /* create an XpmImage from the image */
    ErrorStatus = XpmCreateXpmImageFromImage(display, image, shapeimage,
					     &xpmimage, attributes);
    if (ErrorStatus != XpmSuccess)
	return (ErrorStatus);

    /* create the data from the XpmImage */
    if (attributes) {
	xpmSetInfo(&info, attributes);
	ErrorStatus = XpmCreateDataFromXpmImage(data_return, &xpmimage, &info);
    } else
	ErrorStatus = XpmCreateDataFromXpmImage(data_return, &xpmimage, NULL);

    /* free the XpmImage */
    XpmFreeXpmImage(&xpmimage);

    return (ErrorStatus);
}
