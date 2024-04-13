bool ACodec::describeDefaultColorFormat(DescribeColorFormatParams &params) {
 MediaImage &image = params.sMediaImage;
    memset(&image, 0, sizeof(image));

    image.mType = MediaImage::MEDIA_IMAGE_TYPE_UNKNOWN;
    image.mNumPlanes = 0;

 const OMX_COLOR_FORMATTYPE fmt = params.eColorFormat;
    image.mWidth = params.nFrameWidth;
    image.mHeight = params.nFrameHeight;

 if (fmt != OMX_COLOR_FormatYUV420Planar &&
        fmt != OMX_COLOR_FormatYUV420PackedPlanar &&
        fmt != OMX_COLOR_FormatYUV420SemiPlanar &&
        fmt != OMX_COLOR_FormatYUV420PackedSemiPlanar &&
        fmt != HAL_PIXEL_FORMAT_YV12) {
        ALOGW("do not know color format 0x%x = %d", fmt, fmt);
 return false;
 }

 if (params.nStride != 0 && params.nSliceHeight == 0) {
        ALOGW("using sliceHeight=%u instead of what codec advertised (=0)",
                params.nFrameHeight);
        params.nSliceHeight = params.nFrameHeight;
 }

 if (params.nStride == 0 || params.nSliceHeight == 0) {
        ALOGW("cannot describe color format 0x%x = %d with stride=%u and sliceHeight=%u",
                fmt, fmt, params.nStride, params.nSliceHeight);
 return false;
 }

    image.mType = MediaImage::MEDIA_IMAGE_TYPE_YUV;
    image.mNumPlanes = 3;
    image.mBitDepth = 8;
    image.mPlane[image.Y].mOffset = 0;
    image.mPlane[image.Y].mColInc = 1;
    image.mPlane[image.Y].mRowInc = params.nStride;
    image.mPlane[image.Y].mHorizSubsampling = 1;
    image.mPlane[image.Y].mVertSubsampling = 1;

 switch ((int)fmt) {
 case HAL_PIXEL_FORMAT_YV12:
 if (params.bUsingNativeBuffers) {
 size_t ystride = align(params.nStride, 16);
 size_t cstride = align(params.nStride / 2, 16);
                image.mPlane[image.Y].mRowInc = ystride;

                image.mPlane[image.V].mOffset = ystride * params.nSliceHeight;
                image.mPlane[image.V].mColInc = 1;
                image.mPlane[image.V].mRowInc = cstride;
                image.mPlane[image.V].mHorizSubsampling = 2;
                image.mPlane[image.V].mVertSubsampling = 2;

                image.mPlane[image.U].mOffset = image.mPlane[image.V].mOffset
 + (cstride * params.nSliceHeight / 2);
                image.mPlane[image.U].mColInc = 1;
                image.mPlane[image.U].mRowInc = cstride;
                image.mPlane[image.U].mHorizSubsampling = 2;
                image.mPlane[image.U].mVertSubsampling = 2;
 break;
 } else {
 }

 case OMX_COLOR_FormatYUV420Planar:
 case OMX_COLOR_FormatYUV420PackedPlanar:
            image.mPlane[image.U].mOffset = params.nStride * params.nSliceHeight;
            image.mPlane[image.U].mColInc = 1;
            image.mPlane[image.U].mRowInc = params.nStride / 2;
            image.mPlane[image.U].mHorizSubsampling = 2;
            image.mPlane[image.U].mVertSubsampling = 2;

            image.mPlane[image.V].mOffset = image.mPlane[image.U].mOffset
 + (params.nStride * params.nSliceHeight / 4);
            image.mPlane[image.V].mColInc = 1;
            image.mPlane[image.V].mRowInc = params.nStride / 2;
            image.mPlane[image.V].mHorizSubsampling = 2;
            image.mPlane[image.V].mVertSubsampling = 2;
 break;

 case OMX_COLOR_FormatYUV420SemiPlanar:
 case OMX_COLOR_FormatYUV420PackedSemiPlanar:
            image.mPlane[image.U].mOffset = params.nStride * params.nSliceHeight;
            image.mPlane[image.U].mColInc = 2;
            image.mPlane[image.U].mRowInc = params.nStride;
            image.mPlane[image.U].mHorizSubsampling = 2;
            image.mPlane[image.U].mVertSubsampling = 2;

            image.mPlane[image.V].mOffset = image.mPlane[image.U].mOffset + 1;
            image.mPlane[image.V].mColInc = 2;
            image.mPlane[image.V].mRowInc = params.nStride;
            image.mPlane[image.V].mHorizSubsampling = 2;
            image.mPlane[image.V].mVertSubsampling = 2;
 break;

 default:
            TRESPASS();
 }
 return true;
}
