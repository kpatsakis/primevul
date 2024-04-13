close_ps(PG_FUNCTION_ARGS)
{
	Point	   *pt = PG_GETARG_POINT_P(0);
	LSEG	   *lseg = PG_GETARG_LSEG_P(1);
	Point	   *result = NULL;
	LINE	   *tmp;
	double		invm;
	int			xh,
				yh;

#ifdef GEODEBUG
	printf("close_sp:pt->x %f pt->y %f\nlseg(0).x %f lseg(0).y %f  lseg(1).x %f lseg(1).y %f\n",
		   pt->x, pt->y, lseg->p[0].x, lseg->p[0].y,
		   lseg->p[1].x, lseg->p[1].y);
#endif

	/* xh (or yh) is the index of upper x( or y) end point of lseg */
	/* !xh (or !yh) is the index of lower x( or y) end point of lseg */
	xh = lseg->p[0].x < lseg->p[1].x;
	yh = lseg->p[0].y < lseg->p[1].y;

	if (FPeq(lseg->p[0].x, lseg->p[1].x))		/* vertical? */
	{
#ifdef GEODEBUG
		printf("close_ps- segment is vertical\n");
#endif
		/* first check if point is below or above the entire lseg. */
		if (pt->y < lseg->p[!yh].y)
			result = point_copy(&lseg->p[!yh]); /* below the lseg */
		else if (pt->y > lseg->p[yh].y)
			result = point_copy(&lseg->p[yh]);	/* above the lseg */
		if (result != NULL)
			PG_RETURN_POINT_P(result);

		/* point lines along (to left or right) of the vertical lseg. */

		result = (Point *) palloc(sizeof(Point));
		result->x = lseg->p[0].x;
		result->y = pt->y;
		PG_RETURN_POINT_P(result);
	}
	else if (FPeq(lseg->p[0].y, lseg->p[1].y))	/* horizontal? */
	{
#ifdef GEODEBUG
		printf("close_ps- segment is horizontal\n");
#endif
		/* first check if point is left or right of the entire lseg. */
		if (pt->x < lseg->p[!xh].x)
			result = point_copy(&lseg->p[!xh]); /* left of the lseg */
		else if (pt->x > lseg->p[xh].x)
			result = point_copy(&lseg->p[xh]);	/* right of the lseg */
		if (result != NULL)
			PG_RETURN_POINT_P(result);

		/* point lines along (at top or below) the horiz. lseg. */
		result = (Point *) palloc(sizeof(Point));
		result->x = pt->x;
		result->y = lseg->p[0].y;
		PG_RETURN_POINT_P(result);
	}

	/*
	 * vert. and horiz. cases are down, now check if the closest point is one
	 * of the end points or someplace on the lseg.
	 */

	invm = -1.0 / point_sl(&(lseg->p[0]), &(lseg->p[1]));
	tmp = line_construct_pm(&lseg->p[!yh], invm);		/* lower edge of the
														 * "band" */
	if (pt->y < (tmp->A * pt->x + tmp->C))
	{							/* we are below the lower edge */
		result = point_copy(&lseg->p[!yh]);		/* below the lseg, take lower
												 * end pt */
#ifdef GEODEBUG
		printf("close_ps below: tmp A %f  B %f   C %f    m %f\n",
			   tmp->A, tmp->B, tmp->C, tmp->m);
#endif
		PG_RETURN_POINT_P(result);
	}
	tmp = line_construct_pm(&lseg->p[yh], invm);		/* upper edge of the
														 * "band" */
	if (pt->y > (tmp->A * pt->x + tmp->C))
	{							/* we are below the lower edge */
		result = point_copy(&lseg->p[yh]);		/* above the lseg, take higher
												 * end pt */
#ifdef GEODEBUG
		printf("close_ps above: tmp A %f  B %f   C %f    m %f\n",
			   tmp->A, tmp->B, tmp->C, tmp->m);
#endif
		PG_RETURN_POINT_P(result);
	}

	/*
	 * at this point the "normal" from point will hit lseg. The closet point
	 * will be somewhere on the lseg
	 */
	tmp = line_construct_pm(pt, invm);
#ifdef GEODEBUG
	printf("close_ps- tmp A %f  B %f   C %f    m %f\n",
		   tmp->A, tmp->B, tmp->C, tmp->m);
#endif
	result = interpt_sl(lseg, tmp);
	Assert(result != NULL);
#ifdef GEODEBUG
	printf("close_ps- result.x %f  result.y %f\n", result->x, result->y);
#endif
	PG_RETURN_POINT_P(result);
}
