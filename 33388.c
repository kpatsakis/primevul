static void fbFetchTransformed(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
    FbBits     *bits;
    FbStride    stride;
    int         bpp;
    int         xoff, yoff, dx, dy;
    fetchPixelProc   fetch;
    PictVector	v;
    PictVector  unit;
    int         i;
    BoxRec	box;
    miIndexedPtr indexed = (miIndexedPtr) pict->pFormat->index.devPrivate;
    Bool affine = TRUE;

    fetch = fetchPixelProcForPicture(pict);

    fbGetDrawable(pict->pDrawable, bits, stride, bpp, xoff, yoff);
    x += xoff;
    y += yoff;

    dx = pict->pDrawable->x;
    dy = pict->pDrawable->y;

    /* reference point is the center of the pixel */
    v.vector[0] = IntToxFixed(x - dx) + xFixed1 / 2;
    v.vector[1] = IntToxFixed(y - dy) + xFixed1 / 2;
    v.vector[2] = xFixed1;

    /* when using convolution filters one might get here without a transform */
    if (pict->transform) {
        if (!PictureTransformPoint3d (pict->transform, &v)) {
            fbFinishAccess (pict->pDrawable);
            return;
        }
        unit.vector[0] = pict->transform->matrix[0][0];
        unit.vector[1] = pict->transform->matrix[1][0];
        unit.vector[2] = pict->transform->matrix[2][0];
        affine = v.vector[2] == xFixed1 && unit.vector[2] == 0;
    } else {
        unit.vector[0] = xFixed1;
        unit.vector[1] = 0;
        unit.vector[2] = 0;
    }

    if (pict->filter == PictFilterNearest)
    {
        if (pict->repeatType == RepeatNormal) {
            if (REGION_NUM_RECTS(pict->pCompositeClip) == 1) {
                for (i = 0; i < width; ++i) {
		    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    if (!affine) {
				y = MOD(DIV(v.vector[1],v.vector[2]), pict->pDrawable->height);
				x = MOD(DIV(v.vector[0],v.vector[2]), pict->pDrawable->width);
			    } else {
				y = MOD(v.vector[1]>>16, pict->pDrawable->height);
				x = MOD(v.vector[0]>>16, pict->pDrawable->width);
			    }
			    WRITE(buffer + i, fetch(bits + (y + dy)*stride, x + dx, indexed));
			}
		    }

                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            } else {
                for (i = 0; i < width; ++i) {
		    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    if (!affine) {
				y = MOD(DIV(v.vector[1],v.vector[2]), pict->pDrawable->height);
				x = MOD(DIV(v.vector[0],v.vector[2]), pict->pDrawable->width);
			    } else {
				y = MOD(v.vector[1]>>16, pict->pDrawable->height);
				x = MOD(v.vector[0]>>16, pict->pDrawable->width);
			    }
			    if (POINT_IN_REGION (0, pict->pCompositeClip, x, y, &box))
				WRITE(buffer + i, fetch(bits + (y + dy)*stride, x + dx, indexed));
			    else
				WRITE(buffer + i, 0);
			}
		    }

                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            }
        } else {
            if (REGION_NUM_RECTS(pict->pCompositeClip) == 1) {
                box = pict->pCompositeClip->extents;
                for (i = 0; i < width; ++i) {
		    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    if (!affine) {
				y = DIV(v.vector[1],v.vector[2]);
				x = DIV(v.vector[0],v.vector[2]);
			    } else {
				y = v.vector[1]>>16;
				x = v.vector[0]>>16;
			    }
			    WRITE(buffer + i, ((x < box.x1-dx) | (x >= box.x2-dx) | (y < box.y1-dy) | (y >= box.y2-dy)) ?
				  0 : fetch(bits + (y + dy)*stride, x + dx, indexed));
			}
		    }
                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            } else {
                for (i = 0; i < width; ++i) {
                    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    if (!affine) {
				y = DIV(v.vector[1],v.vector[2]);
				x = DIV(v.vector[0],v.vector[2]);
			    } else {
				y = v.vector[1]>>16;
				x = v.vector[0]>>16;
			    }
			    if (POINT_IN_REGION (0, pict->pCompositeClip, x + dx, y + dy, &box))
				WRITE(buffer + i, fetch(bits + (y + dy)*stride, x + dx, indexed));
			    else
				WRITE(buffer + i, 0);
			}
		    }
                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            }
        }
    } else if (pict->filter == PictFilterBilinear) {
        /* adjust vector for maximum contribution at 0.5, 0.5 of each texel. */
        v.vector[0] -= v.vector[2] / 2;
        v.vector[1] -= v.vector[2] / 2;
        unit.vector[0] -= unit.vector[2] / 2;
        unit.vector[1] -= unit.vector[2] / 2;

        if (pict->repeatType == RepeatNormal) {
            if (REGION_NUM_RECTS(pict->pCompositeClip) == 1) {
                for (i = 0; i < width; ++i) {
                    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    int x1, x2, y1, y2, distx, idistx, disty, idisty;
			    FbBits *b;
			    CARD32 tl, tr, bl, br, r;
			    CARD32 ft, fb;

			    if (!affine) {
				xFixed_48_16 div;
				div = ((xFixed_48_16)v.vector[0] << 16)/v.vector[2];
				x1 = div >> 16;
				distx = ((xFixed)div >> 8) & 0xff;
				div = ((xFixed_48_16)v.vector[1] << 16)/v.vector[2];
				y1 = div >> 16;
				disty = ((xFixed)div >> 8) & 0xff;
			    } else {
				x1 = v.vector[0] >> 16;
				distx = (v.vector[0] >> 8) & 0xff;
				y1 = v.vector[1] >> 16;
				disty = (v.vector[1] >> 8) & 0xff;
			    }
			    x2 = x1 + 1;
			    y2 = y1 + 1;

			    idistx = 256 - distx;
			    idisty = 256 - disty;

			    x1 = MOD (x1, pict->pDrawable->width);
			    x2 = MOD (x2, pict->pDrawable->width);
			    y1 = MOD (y1, pict->pDrawable->height);
			    y2 = MOD (y2, pict->pDrawable->height);

			    b = bits + (y1 + dy)*stride;

			    tl = fetch(b, x1 + dx, indexed);
			    tr = fetch(b, x2 + dx, indexed);
			    b = bits + (y2 + dy)*stride;
			    bl = fetch(b, x1 + dx, indexed);
			    br = fetch(b, x2 + dx, indexed);

			    ft = FbGet8(tl,0) * idistx + FbGet8(tr,0) * distx;
			    fb = FbGet8(bl,0) * idistx + FbGet8(br,0) * distx;
			    r = (((ft * idisty + fb * disty) >> 16) & 0xff);
			    ft = FbGet8(tl,8) * idistx + FbGet8(tr,8) * distx;
			    fb = FbGet8(bl,8) * idistx + FbGet8(br,8) * distx;
			    r |= (((ft * idisty + fb * disty) >> 8) & 0xff00);
			    ft = FbGet8(tl,16) * idistx + FbGet8(tr,16) * distx;
			    fb = FbGet8(bl,16) * idistx + FbGet8(br,16) * distx;
			    r |= (((ft * idisty + fb * disty)) & 0xff0000);
			    ft = FbGet8(tl,24) * idistx + FbGet8(tr,24) * distx;
			    fb = FbGet8(bl,24) * idistx + FbGet8(br,24) * distx;
			    r |= (((ft * idisty + fb * disty) << 8) & 0xff000000);
			    WRITE(buffer + i, r);
			}
		    }
                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            } else {
                for (i = 0; i < width; ++i) {
		    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    int x1, x2, y1, y2, distx, idistx, disty, idisty;
			    FbBits *b;
			    CARD32 tl, tr, bl, br, r;
			    CARD32 ft, fb;

			    if (!affine) {
				xFixed_48_16 div;
				div = ((xFixed_48_16)v.vector[0] << 16)/v.vector[2];
				x1 = div >> 16;
				distx = ((xFixed)div >> 8) & 0xff;
				div = ((xFixed_48_16)v.vector[1] << 16)/v.vector[2];
				y1 = div >> 16;
				disty = ((xFixed)div >> 8) & 0xff;
			    } else {
				x1 = v.vector[0] >> 16;
				distx = (v.vector[0] >> 8) & 0xff;
				y1 = v.vector[1] >> 16;
				disty = (v.vector[1] >> 8) & 0xff;
			    }
			    x2 = x1 + 1;
			    y2 = y1 + 1;

			    idistx = 256 - distx;
			    idisty = 256 - disty;

			    x1 = MOD (x1, pict->pDrawable->width);
			    x2 = MOD (x2, pict->pDrawable->width);
			    y1 = MOD (y1, pict->pDrawable->height);
			    y2 = MOD (y2, pict->pDrawable->height);

			    b = bits + (y1 + dy)*stride;

			    tl = POINT_IN_REGION(0, pict->pCompositeClip, x1 + dx, y1 + dy, &box)
				? fetch(b, x1 + dx, indexed) : 0;
			    tr = POINT_IN_REGION(0, pict->pCompositeClip, x2 + dx, y1 + dy, &box)
				? fetch(b, x2 + dx, indexed) : 0;
			    b = bits + (y2 + dy)*stride;
			    bl = POINT_IN_REGION(0, pict->pCompositeClip, x1 + dx, y2 + dy, &box)
				? fetch(b, x1 + dx, indexed) : 0;
			    br = POINT_IN_REGION(0, pict->pCompositeClip, x2 + dx, y2 + dy, &box)
				? fetch(b, x2 + dx, indexed) : 0;

			    ft = FbGet8(tl,0) * idistx + FbGet8(tr,0) * distx;
			    fb = FbGet8(bl,0) * idistx + FbGet8(br,0) * distx;
			    r = (((ft * idisty + fb * disty) >> 16) & 0xff);
			    ft = FbGet8(tl,8) * idistx + FbGet8(tr,8) * distx;
			    fb = FbGet8(bl,8) * idistx + FbGet8(br,8) * distx;
			    r |= (((ft * idisty + fb * disty) >> 8) & 0xff00);
			    ft = FbGet8(tl,16) * idistx + FbGet8(tr,16) * distx;
			    fb = FbGet8(bl,16) * idistx + FbGet8(br,16) * distx;
			    r |= (((ft * idisty + fb * disty)) & 0xff0000);
			    ft = FbGet8(tl,24) * idistx + FbGet8(tr,24) * distx;
			    fb = FbGet8(bl,24) * idistx + FbGet8(br,24) * distx;
			    r |= (((ft * idisty + fb * disty) << 8) & 0xff000000);
			    WRITE(buffer + i, r);
			}
		    }

                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            }
        } else {
            if (REGION_NUM_RECTS(pict->pCompositeClip) == 1) {
                box = pict->pCompositeClip->extents;
                for (i = 0; i < width; ++i) {
		    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    int x1, x2, y1, y2, distx, idistx, disty, idisty, x_off;
			    FbBits *b;
			    CARD32 tl, tr, bl, br, r;
			    Bool x1_out, x2_out, y1_out, y2_out;
			    CARD32 ft, fb;

			    if (!affine) {
				xFixed_48_16 div;
				div = ((xFixed_48_16)v.vector[0] << 16)/v.vector[2];
				x1 = div >> 16;
				distx = ((xFixed)div >> 8) & 0xff;
				div = ((xFixed_48_16)v.vector[1] << 16)/v.vector[2];
				y1 = div >> 16;
				disty = ((xFixed)div >> 8) & 0xff;
			    } else {
				x1 = v.vector[0] >> 16;
				distx = (v.vector[0] >> 8) & 0xff;
				y1 = v.vector[1] >> 16;
				disty = (v.vector[1] >> 8) & 0xff;
			    }
			    x2 = x1 + 1;
			    y2 = y1 + 1;

			    idistx = 256 - distx;
			    idisty = 256 - disty;

			    b = bits + (y1 + dy)*stride;
			    x_off = x1 + dx;

			    x1_out = (x1 < box.x1-dx) | (x1 >= box.x2-dx);
			    x2_out = (x2 < box.x1-dx) | (x2 >= box.x2-dx);
			    y1_out = (y1 < box.y1-dy) | (y1 >= box.y2-dy);
			    y2_out = (y2 < box.y1-dy) | (y2 >= box.y2-dy);

			    tl = x1_out|y1_out ? 0 : fetch(b, x_off, indexed);
			    tr = x2_out|y1_out ? 0 : fetch(b, x_off + 1, indexed);
			    b += stride;
			    bl = x1_out|y2_out ? 0 : fetch(b, x_off, indexed);
			    br = x2_out|y2_out ? 0 : fetch(b, x_off + 1, indexed);

			    ft = FbGet8(tl,0) * idistx + FbGet8(tr,0) * distx;
			    fb = FbGet8(bl,0) * idistx + FbGet8(br,0) * distx;
			    r = (((ft * idisty + fb * disty) >> 16) & 0xff);
			    ft = FbGet8(tl,8) * idistx + FbGet8(tr,8) * distx;
			    fb = FbGet8(bl,8) * idistx + FbGet8(br,8) * distx;
			    r |= (((ft * idisty + fb * disty) >> 8) & 0xff00);
			    ft = FbGet8(tl,16) * idistx + FbGet8(tr,16) * distx;
			    fb = FbGet8(bl,16) * idistx + FbGet8(br,16) * distx;
			    r |= (((ft * idisty + fb * disty)) & 0xff0000);
			    ft = FbGet8(tl,24) * idistx + FbGet8(tr,24) * distx;
			    fb = FbGet8(bl,24) * idistx + FbGet8(br,24) * distx;
			    r |= (((ft * idisty + fb * disty) << 8) & 0xff000000);
			    WRITE(buffer + i, r);
			}
		    }

                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            } else {
                for (i = 0; i < width; ++i) {
                    if (!mask || mask[i] & maskBits)
		    {
			if (!v.vector[2]) {
			    WRITE(buffer + i, 0);
			} else {
			    int x1, x2, y1, y2, distx, idistx, disty, idisty, x_off;
			    FbBits *b;
			    CARD32 tl, tr, bl, br, r;
			    CARD32 ft, fb;

			    if (!affine) {
				xFixed_48_16 div;
				div = ((xFixed_48_16)v.vector[0] << 16)/v.vector[2];
				x1 = div >> 16;
				distx = ((xFixed)div >> 8) & 0xff;
				div = ((xFixed_48_16)v.vector[1] << 16)/v.vector[2];
				y1 = div >> 16;
				disty = ((xFixed)div >> 8) & 0xff;
			    } else {
				x1 = v.vector[0] >> 16;
				distx = (v.vector[0] >> 8) & 0xff;
				y1 = v.vector[1] >> 16;
				disty = (v.vector[1] >> 8) & 0xff;
			    }
			    x2 = x1 + 1;
			    y2 = y1 + 1;

			    idistx = 256 - distx;
			    idisty = 256 - disty;

			    b = bits + (y1 + dy)*stride;
			    x_off = x1 + dx;

			    tl = POINT_IN_REGION(0, pict->pCompositeClip, x1 + dx, y1 + dy, &box)
				? fetch(b, x_off, indexed) : 0;
			    tr = POINT_IN_REGION(0, pict->pCompositeClip, x2 + dx, y1 + dy, &box)
				? fetch(b, x_off + 1, indexed) : 0;
			    b += stride;
			    bl = POINT_IN_REGION(0, pict->pCompositeClip, x1 + dx, y2 + dy, &box)
				? fetch(b, x_off, indexed) : 0;
			    br = POINT_IN_REGION(0, pict->pCompositeClip, x2 + dx, y2 + dy, &box)
				? fetch(b, x_off + 1, indexed) : 0;

			    ft = FbGet8(tl,0) * idistx + FbGet8(tr,0) * distx;
			    fb = FbGet8(bl,0) * idistx + FbGet8(br,0) * distx;
			    r = (((ft * idisty + fb * disty) >> 16) & 0xff);
			    ft = FbGet8(tl,8) * idistx + FbGet8(tr,8) * distx;
			    fb = FbGet8(bl,8) * idistx + FbGet8(br,8) * distx;
			    r |= (((ft * idisty + fb * disty) >> 8) & 0xff00);
			    ft = FbGet8(tl,16) * idistx + FbGet8(tr,16) * distx;
			    fb = FbGet8(bl,16) * idistx + FbGet8(br,16) * distx;
			    r |= (((ft * idisty + fb * disty)) & 0xff0000);
			    ft = FbGet8(tl,24) * idistx + FbGet8(tr,24) * distx;
			    fb = FbGet8(bl,24) * idistx + FbGet8(br,24) * distx;
			    r |= (((ft * idisty + fb * disty) << 8) & 0xff000000);
			    WRITE(buffer + i, r);
			}
		    }

                    v.vector[0] += unit.vector[0];
                    v.vector[1] += unit.vector[1];
                    v.vector[2] += unit.vector[2];
                }
            }
        }
    } else if (pict->filter == PictFilterConvolution) {
        xFixed *params = pict->filter_params;
        INT32 cwidth = xFixedToInt(params[0]);
        INT32 cheight = xFixedToInt(params[1]);
        int xoff = (params[0] - xFixed1) >> 1;
	int yoff = (params[1] - xFixed1) >> 1;
        params += 2;
        for (i = 0; i < width; ++i) {
	    if (!mask || mask[i] & maskBits)
	    {
		if (!v.vector[2]) {
		    WRITE(buffer + i, 0);
		} else {
		    int x1, x2, y1, y2, x, y;
		    INT32 srtot, sgtot, sbtot, satot;
		    xFixed *p = params;

		    if (!affine) {
			xFixed_48_16 tmp;
			tmp = ((xFixed_48_16)v.vector[0] << 16)/v.vector[2] - xoff;
			x1 = xFixedToInt(tmp);
			tmp = ((xFixed_48_16)v.vector[1] << 16)/v.vector[2] - yoff;
			y1 = xFixedToInt(tmp);
		    } else {
			x1 = xFixedToInt(v.vector[0] - xoff);
			y1 = xFixedToInt(v.vector[1] - yoff);
		    }
		    x2 = x1 + cwidth;
		    y2 = y1 + cheight;

		    srtot = sgtot = sbtot = satot = 0;

		    for (y = y1; y < y2; y++) {
			int ty = (pict->repeatType == RepeatNormal) ? MOD (y, pict->pDrawable->height) : y;
			for (x = x1; x < x2; x++) {
			    if (*p) {
				int tx = (pict->repeatType == RepeatNormal) ? MOD (x, pict->pDrawable->width) : x;
				if (POINT_IN_REGION (0, pict->pCompositeClip, tx + dx, ty + dy, &box)) {
				    FbBits *b = bits + (ty + dy)*stride;
				    CARD32 c = fetch(b, tx + dx, indexed);

				    srtot += Red(c) * *p;
				    sgtot += Green(c) * *p;
				    sbtot += Blue(c) * *p;
				    satot += Alpha(c) * *p;
				}
			    }
			    p++;
			}
		    }

		    satot >>= 16;
		    srtot >>= 16;
		    sgtot >>= 16;
		    sbtot >>= 16;

		    if (satot < 0) satot = 0; else if (satot > 0xff) satot = 0xff;
		    if (srtot < 0) srtot = 0; else if (srtot > 0xff) srtot = 0xff;
		    if (sgtot < 0) sgtot = 0; else if (sgtot > 0xff) sgtot = 0xff;
		    if (sbtot < 0) sbtot = 0; else if (sbtot > 0xff) sbtot = 0xff;

		    WRITE(buffer + i, ((satot << 24) |
				       (srtot << 16) |
				       (sgtot <<  8) |
				       (sbtot       )));
		}
	    }
            v.vector[0] += unit.vector[0];
            v.vector[1] += unit.vector[1];
            v.vector[2] += unit.vector[2];
        }
    }

    fbFinishAccess (pict->pDrawable);
}
