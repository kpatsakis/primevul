line_construct_pts(LINE *line, Point *pt1, Point *pt2)
{
	if (FPeq(pt1->x, pt2->x))
	{							/* vertical */
		/* use "x = C" */
		line->A = -1;
		line->B = 0;
		line->C = pt1->x;
#ifdef GEODEBUG
		printf("line_construct_pts- line is vertical\n");
#endif
	}
	else if (FPeq(pt1->y, pt2->y))
	{							/* horizontal */
		/* use "y = C" */
		line->A = 0;
		line->B = -1;
		line->C = pt1->y;
#ifdef GEODEBUG
		printf("line_construct_pts- line is horizontal\n");
#endif
	}
	else
	{
		/* use "mx - y + yinter = 0" */
		line->A = (pt2->y - pt1->y) / (pt2->x - pt1->x);
		line->B = -1.0;
		line->C = pt1->y - line->A * pt1->x;
		/* on some platforms, the preceding expression tends to produce -0 */
		if (line->C == 0.0)
			line->C = 0.0;
#ifdef GEODEBUG
		printf("line_construct_pts- line is neither vertical nor horizontal (diffs x=%.*g, y=%.*g\n",
			   DBL_DIG, (pt2->x - pt1->x), DBL_DIG, (pt2->y - pt1->y));
#endif
	}
}
