unsigned ImageInputType::height() const
{
    RefPtrWillBeRawPtr<HTMLInputElement> element(this->element());

    if (!element->layoutObject()) {
        unsigned height;
        if (parseHTMLNonNegativeInteger(element->fastGetAttribute(heightAttr), height))
            return height;

        HTMLImageLoader* imageLoader = element->imageLoader();
        if (imageLoader && imageLoader->image())
            return imageLoader->image()->imageSize(LayoutObject::shouldRespectImageOrientation(nullptr), 1).height();
    }

    element->document().updateLayout();

    LayoutBox* box = element->layoutBox();
    return box ? adjustForAbsoluteZoom(box->contentHeight(), box) : 0;
}
