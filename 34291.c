PanoramiXRenderInit (void)
{
    int	    i;
    
    XRT_PICTURE = CreateNewResourceType (XineramaDeleteResource,
					 "XineramaPicture");
    if (RenderErrBase)
	SetResourceTypeErrorValue(XRT_PICTURE, RenderErrBase + BadPicture);
    for (i = 0; i < RenderNumberRequests; i++)
	PanoramiXSaveRenderVector[i] = ProcRenderVector[i];
    /*
     * Stuff in Xinerama aware request processing hooks
     */
    ProcRenderVector[X_RenderCreatePicture] = PanoramiXRenderCreatePicture;
    ProcRenderVector[X_RenderChangePicture] = PanoramiXRenderChangePicture;
    ProcRenderVector[X_RenderSetPictureTransform] = PanoramiXRenderSetPictureTransform;
    ProcRenderVector[X_RenderSetPictureFilter] = PanoramiXRenderSetPictureFilter;
    ProcRenderVector[X_RenderSetPictureClipRectangles] = PanoramiXRenderSetPictureClipRectangles;
    ProcRenderVector[X_RenderFreePicture] = PanoramiXRenderFreePicture;
    ProcRenderVector[X_RenderComposite] = PanoramiXRenderComposite;
    ProcRenderVector[X_RenderCompositeGlyphs8] = PanoramiXRenderCompositeGlyphs;
    ProcRenderVector[X_RenderCompositeGlyphs16] = PanoramiXRenderCompositeGlyphs;
    ProcRenderVector[X_RenderCompositeGlyphs32] = PanoramiXRenderCompositeGlyphs;
    ProcRenderVector[X_RenderFillRectangles] = PanoramiXRenderFillRectangles;

    ProcRenderVector[X_RenderTrapezoids] = PanoramiXRenderTrapezoids;
    ProcRenderVector[X_RenderTriangles] = PanoramiXRenderTriangles;
    ProcRenderVector[X_RenderTriStrip] = PanoramiXRenderTriStrip;
    ProcRenderVector[X_RenderTriFan] = PanoramiXRenderTriFan;
    ProcRenderVector[X_RenderAddTraps] = PanoramiXRenderAddTraps;

    ProcRenderVector[X_RenderCreateSolidFill] = PanoramiXRenderCreateSolidFill;
    ProcRenderVector[X_RenderCreateLinearGradient] = PanoramiXRenderCreateLinearGradient;
    ProcRenderVector[X_RenderCreateRadialGradient] = PanoramiXRenderCreateRadialGradient;
    ProcRenderVector[X_RenderCreateConicalGradient] = PanoramiXRenderCreateConicalGradient;
}
