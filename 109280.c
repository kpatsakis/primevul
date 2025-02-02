static void _php_image_output_ctx(INTERNAL_FUNCTION_PARAMETERS, int image_type, char *tn, void (*func_p)())
{
	zval *imgind;
	char *file = NULL;
	int file_len = 0;
	long quality, basefilter;
	gdImagePtr im;
	int argc = ZEND_NUM_ARGS();
	int q = -1, i;
	int f = -1;
	gdIOCtx *ctx = NULL;
	zval *to_zval = NULL;
	php_stream *stream;

	/* The third (quality) parameter for Wbmp stands for the threshold when called from image2wbmp().
	 * The third (quality) parameter for Wbmp and Xbm stands for the foreground color index when called
	 * from imagey<type>().
	 */
	if (image_type == PHP_GDIMG_TYPE_XBM) {
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rp!|ll", &imgind, &file, &file_len, &quality, &basefilter) == FAILURE) {
			return;
		}
	} else {
		/* PHP_GDIMG_TYPE_GIF
		 * PHP_GDIMG_TYPE_PNG 
		 * PHP_GDIMG_TYPE_JPG 
		 * PHP_GDIMG_TYPE_WBM 
		 * PHP_GDIMG_TYPE_WEBP 
		 * */
		if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r|z/!ll", &imgind, &to_zval, &quality, &basefilter) == FAILURE) {
			return;
		}
	}

	ZEND_FETCH_RESOURCE(im, gdImagePtr, &imgind, -1, "Image", phpi_get_le_gd());

	if (argc >= 3) {
		q = quality; /* or colorindex for foreground of BW images (defaults to black) */
		if (argc == 4) {
			f = basefilter;
		}
	}

	if (argc > 1 && to_zval != NULL) {
		if (Z_TYPE_P(to_zval) == IS_RESOURCE) {
			php_stream_from_zval_no_verify(stream, &to_zval);
			if (stream == NULL) {
				RETURN_FALSE;
			}
		} else if (Z_TYPE_P(to_zval) == IS_STRING) {
			stream = php_stream_open_wrapper(Z_STRVAL_P(to_zval), "wb", REPORT_ERRORS|IGNORE_PATH|IGNORE_URL_WIN, NULL);
			if (stream == NULL) {
				RETURN_FALSE;
			}
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid 2nd parameter, it must a filename or a stream");
			RETURN_FALSE;
		}
	} else {
		ctx = emalloc(sizeof(gdIOCtx));
		ctx->putC = _php_image_output_putc;
		ctx->putBuf = _php_image_output_putbuf;
		ctx->gd_free = _php_image_output_ctxfree;

#if APACHE && defined(CHARSET_EBCDIC)
		/* XXX this is unlikely to work any more thies@thieso.net */
		/* This is a binary file already: avoid EBCDIC->ASCII conversion */
		ap_bsetflag(php3_rqst->connection->client, B_EBCDIC2ASCII, 0);
#endif
	}

	if (!ctx)	{
		ctx = emalloc(sizeof(gdIOCtx));
		ctx->putC = _php_image_stream_putc;
		ctx->putBuf = _php_image_stream_putbuf;
		ctx->gd_free = _php_image_stream_ctxfree;
		ctx->data = (void *)stream;
	}

	switch(image_type) {
		case PHP_GDIMG_CONVERT_WBM:
			if(q<0||q>255) {
				php_error_docref(NULL TSRMLS_CC, E_WARNING, "Invalid threshold value '%d'. It must be between 0 and 255", q);
			}
		case PHP_GDIMG_TYPE_JPG:
			(*func_p)(im, ctx, q);
			break;
		case PHP_GDIMG_TYPE_WEBP:
			if (q == -1) {
				q = 80;
			}
			(*func_p)(im, ctx, q);
			break;
		case PHP_GDIMG_TYPE_PNG:
			(*func_p)(im, ctx, q, f);
			break;
		case PHP_GDIMG_TYPE_XBM:
		case PHP_GDIMG_TYPE_WBM:
			if (argc < 3) {
				for(i=0; i < gdImageColorsTotal(im); i++) {
					if(!gdImageRed(im, i) && !gdImageGreen(im, i) && !gdImageBlue(im, i)) break;
				}
				q = i;
			}
			if (image_type == PHP_GDIMG_TYPE_XBM) {
				(*func_p)(im, file, q, ctx);
			} else {
				(*func_p)(im, q, ctx);
			}
			break;
		default:
			(*func_p)(im, ctx);
			break;
	}

	ctx->gd_free(ctx);

	RETURN_TRUE;
}