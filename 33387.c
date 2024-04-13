static void fbFetchSourcePict(PicturePtr pict, int x, int y, int width, CARD32 *buffer, CARD32 *mask, CARD32 maskBits)
{
     SourcePictPtr   pGradient = pict->pSourcePict;
     GradientWalker  walker;
     CARD32         *end = buffer + width;
 
     _gradient_walker_init (&walker, pGradient, pict->repeat);
    
    if (pGradient->type == SourcePictTypeSolidFill) {
        register CARD32 color = pGradient->solidFill.color;
        while (buffer < end) {
            WRITE(buffer++, color);
        }
    } else if (pGradient->type == SourcePictTypeLinear) {
        PictVector v, unit;
        xFixed_32_32 l;
        xFixed_48_16 dx, dy, a, b, off;
	
        /* reference point is the center of the pixel */
        v.vector[0] = IntToxFixed(x) + xFixed1/2;
        v.vector[1] = IntToxFixed(y) + xFixed1/2;
        v.vector[2] = xFixed1;
        if (pict->transform) {
            if (!PictureTransformPoint3d (pict->transform, &v))
                return;
            unit.vector[0] = pict->transform->matrix[0][0];
            unit.vector[1] = pict->transform->matrix[1][0];
            unit.vector[2] = pict->transform->matrix[2][0];
        } else {
            unit.vector[0] = xFixed1;
            unit.vector[1] = 0;
            unit.vector[2] = 0;
        }

        dx = pGradient->linear.p2.x - pGradient->linear.p1.x;
        dy = pGradient->linear.p2.y - pGradient->linear.p1.y;
        l = dx*dx + dy*dy;
        if (l != 0) {
            a = (dx << 32) / l;
            b = (dy << 32) / l;
            off = (-a*pGradient->linear.p1.x - b*pGradient->linear.p1.y)>>16;
        }
        if (l == 0  || (unit.vector[2] == 0 && v.vector[2] == xFixed1)) {
            xFixed_48_16 inc, t;
            /* affine transformation only */
            if (l == 0) {
                t = 0;
                inc = 0;
            } else {
                t = ((a*v.vector[0] + b*v.vector[1]) >> 16) + off;
                inc = (a * unit.vector[0] + b * unit.vector[1]) >> 16;
            }

	    if (pGradient->linear.class == SourcePictClassVertical)
	    {
		register CARD32 color;

		color = _gradient_walker_pixel( &walker, t );
		while (buffer < end)
		    WRITE(buffer++, color);
	    }
	    else
	    {
                if (!mask) {
                    while (buffer < end)
                    {
                        WRITE(buffer, _gradient_walker_pixel (&walker, t));
                        buffer += 1;
                        t      += inc;
                    }
                } else {
                    while (buffer < end) {
                        if (*mask++ & maskBits)
                        {
                            WRITE(buffer, _gradient_walker_pixel (&walker, t));
                        }
                        buffer += 1;
                        t      += inc;
                    }
                }
	    }
	}
	else /* projective transformation */
	{
	    xFixed_48_16 t;

	    if (pGradient->linear.class == SourcePictClassVertical)
	    {
		register CARD32 color;

		if (v.vector[2] == 0)
		{
		    t = 0;
		}
		else
		{
		    xFixed_48_16 x, y;

		    x = ((xFixed_48_16) v.vector[0] << 16) / v.vector[2];
		    y = ((xFixed_48_16) v.vector[1] << 16) / v.vector[2];
		    t = ((a * x + b * y) >> 16) + off;
		}

 		color = _gradient_walker_pixel( &walker, t );
		while (buffer < end)
		    WRITE(buffer++, color);
	    }
	    else
	    {
		while (buffer < end)
		{
		    if (!mask || *mask++ & maskBits)
		    {
			if (v.vector[2] == 0) {
			    t = 0;
			} else {
			    xFixed_48_16 x, y;
			    x = ((xFixed_48_16)v.vector[0] << 16) / v.vector[2];
			    y = ((xFixed_48_16)v.vector[1] << 16) / v.vector[2];
			    t = ((a*x + b*y) >> 16) + off;
			}
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    v.vector[0] += unit.vector[0];
		    v.vector[1] += unit.vector[1];
		    v.vector[2] += unit.vector[2];
		}
            }
        }
    } else {

/*
 * In the radial gradient problem we are given two circles (c₁,r₁) and
 * (c₂,r₂) that define the gradient itself. Then, for any point p, we
 * must compute the value(s) of t within [0.0, 1.0] representing the
 * circle(s) that would color the point.
 *
 * There are potentially two values of t since the point p can be
 * colored by both sides of the circle, (which happens whenever one
 * circle is not entirely contained within the other).
 *
 * If we solve for a value of t that is outside of [0.0, 1.0] then we
 * use the extend mode (NONE, REPEAT, REFLECT, or PAD) to map to a
 * value within [0.0, 1.0].
 *
 * Here is an illustration of the problem:
 *
 *              p₂
 *           p  •
 *           •   ╲
 *        ·       ╲r₂
 *  p₁ ·           ╲
 *  •              θ╲
 *   ╲             ╌╌•
 *    ╲r₁        ·   c₂
 *    θ╲    ·
 *    ╌╌•
 *      c₁
 *
 * Given (c₁,r₁), (c₂,r₂) and p, we must find an angle θ such that two
 * points p₁ and p₂ on the two circles are collinear with p. Then, the
 * desired value of t is the ratio of the length of p₁p to the length
 * of p₁p₂.
 *
 * So, we have six unknown values: (p₁x, p₁y), (p₂x, p₂y), θ and t.
 * We can also write six equations that constrain the problem:
 *
 * Point p₁ is a distance r₁ from c₁ at an angle of θ:
 *
 *	1. p₁x = c₁x + r₁·cos θ
 *	2. p₁y = c₁y + r₁·sin θ
 *
 * Point p₂ is a distance r₂ from c₂ at an angle of θ:
 *
 *	3. p₂x = c₂x + r2·cos θ
 *	4. p₂y = c₂y + r2·sin θ
 *
 * Point p lies at a fraction t along the line segment p₁p₂:
 *
 *	5. px = t·p₂x + (1-t)·p₁x
 *	6. py = t·p₂y + (1-t)·p₁y
 *
 * To solve, first subtitute 1-4 into 5 and 6:
 *
 * px = t·(c₂x + r₂·cos θ) + (1-t)·(c₁x + r₁·cos θ)
 * py = t·(c₂y + r₂·sin θ) + (1-t)·(c₁y + r₁·sin θ)
 *
 * Then solve each for cos θ and sin θ expressed as a function of t:
 *
 * cos θ = (-(c₂x - c₁x)·t + (px - c₁x)) / ((r₂-r₁)·t + r₁)
 * sin θ = (-(c₂y - c₁y)·t + (py - c₁y)) / ((r₂-r₁)·t + r₁)
 *
 * To simplify this a bit, we define new variables for several of the
 * common terms as shown below:
 *
 *              p₂
 *           p  •
 *           •   ╲
 *        ·  ┆    ╲r₂
 *  p₁ ·     ┆     ╲
 *  •     pdy┆      ╲
 *   ╲       ┆       •c₂
 *    ╲r₁    ┆   ·   ┆
 *     ╲    ·┆       ┆cdy
 *      •╌╌╌╌┴╌╌╌╌╌╌╌┘
 *    c₁  pdx   cdx
 *
 * cdx = (c₂x - c₁x)
 * cdy = (c₂y - c₁y)
 *  dr =  r₂-r₁
 * pdx =  px - c₁x
 * pdy =  py - c₁y
 *
 * Note that cdx, cdy, and dr do not depend on point p at all, so can
 * be pre-computed for the entire gradient. The simplifed equations
 * are now:
 *
 * cos θ = (-cdx·t + pdx) / (dr·t + r₁)
 * sin θ = (-cdy·t + pdy) / (dr·t + r₁)
 *
 * Finally, to get a single function of t and eliminate the last
 * unknown θ, we use the identity sin²θ + cos²θ = 1. First, square
 * each equation, (we knew a quadratic was coming since it must be
 * possible to obtain two solutions in some cases):
 *
 * cos²θ = (cdx²t² - 2·cdx·pdx·t + pdx²) / (dr²·t² + 2·r₁·dr·t + r₁²)
 * sin²θ = (cdy²t² - 2·cdy·pdy·t + pdy²) / (dr²·t² + 2·r₁·dr·t + r₁²)
 *
 * Then add both together, set the result equal to 1, and express as a
 * standard quadratic equation in t of the form At² + Bt + C = 0
 *
 * (cdx² + cdy² - dr²)·t² - 2·(cdx·pdx + cdy·pdy + r₁·dr)·t + (pdx² + pdy² - r₁²) = 0
 *
 * In other words:
 *
 * A = cdx² + cdy² - dr²
 * B = -2·(pdx·cdx + pdy·cdy + r₁·dr)
 * C = pdx² + pdy² - r₁²
 *
 * And again, notice that A does not depend on p, so can be
 * precomputed. From here we just use the quadratic formula to solve
 * for t:
 *
 * t = (-2·B ± ⎷(B² - 4·A·C)) / 2·A
 */
        /* radial or conical */
        Bool affine = TRUE;
        double cx = 1.;
        double cy = 0.;
        double cz = 0.;
	double rx = x + 0.5;
	double ry = y + 0.5;
        double rz = 1.;

        if (pict->transform) {
            PictVector v;
            /* reference point is the center of the pixel */
            v.vector[0] = IntToxFixed(x) + xFixed1/2;
            v.vector[1] = IntToxFixed(y) + xFixed1/2;
            v.vector[2] = xFixed1;
            if (!PictureTransformPoint3d (pict->transform, &v))
                return;

            cx = pict->transform->matrix[0][0]/65536.;
            cy = pict->transform->matrix[1][0]/65536.;
            cz = pict->transform->matrix[2][0]/65536.;
            rx = v.vector[0]/65536.;
            ry = v.vector[1]/65536.;
            rz = v.vector[2]/65536.;
            affine = pict->transform->matrix[2][0] == 0 && v.vector[2] == xFixed1;
        }

        if (pGradient->type == SourcePictTypeRadial) {
	    PictRadialGradient *radial;
	    radial = &pGradient->radial;
            if (affine) {
                while (buffer < end) {
		    if (!mask || *mask++ & maskBits)
		    {
			double pdx, pdy;
			double B, C;
			double det;
			double c1x = radial->c1.x / 65536.0;
			double c1y = radial->c1.y / 65536.0;
			double r1  = radial->c1.radius / 65536.0;
                        xFixed_48_16 t;

			pdx = rx - c1x;
			pdy = ry - c1y;

			B = -2 * (  pdx * radial->cdx
				  + pdy * radial->cdy
				  + r1 * radial->dr);
			C = (pdx * pdx + pdy * pdy - r1 * r1);

                        det = (B * B) - (4 * radial->A * C);
			if (det < 0.0)
			    det = 0.0;

			if (radial->A < 0)
			    t = (xFixed_48_16) ((- B - sqrt(det)) / (2.0 * radial->A) * 65536);
			else
			    t = (xFixed_48_16) ((- B + sqrt(det)) / (2.0 * radial->A) * 65536);
			
 			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    
                    rx += cx;
                    ry += cy;
                }
            } else {
		/* projective */
                while (buffer < end) {
		    if (!mask || *mask++ & maskBits)
		    {
			double pdx, pdy;
			double B, C;
			double det;
			double c1x = radial->c1.x / 65536.0;
			double c1y = radial->c1.y / 65536.0;
			double r1  = radial->c1.radius / 65536.0;
                        xFixed_48_16 t;
			double x, y;

			if (rz != 0) {
			    x = rx/rz;
			    y = ry/rz;
			} else {
			    x = y = 0.;
			}
			
			pdx = x - c1x;
			pdy = y - c1y;

			B = -2 * (  pdx * radial->cdx
				  + pdy * radial->cdy
				  + r1 * radial->dr);
			C = (pdx * pdx + pdy * pdy - r1 * r1);

                        det = (B * B) - (4 * radial->A * C);
			if (det < 0.0)
			    det = 0.0;

			if (radial->A < 0)
			    t = (xFixed_48_16) ((- B - sqrt(det)) / (2.0 * radial->A) * 65536);
			else
			    t = (xFixed_48_16) ((- B + sqrt(det)) / (2.0 * radial->A) * 65536);
			
 			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    ++buffer;
		    
                    rx += cx;
                    ry += cy;
		    rz += cz;
                }
            }
        } else /* SourcePictTypeConical */ {
            double a = pGradient->conical.angle/(180.*65536);
            if (affine) {
                rx -= pGradient->conical.center.x/65536.;
                ry -= pGradient->conical.center.y/65536.;

                while (buffer < end) {
		    double angle;

                    if (!mask || *mask++ & maskBits)
		    {
                        xFixed_48_16   t;
			
                        angle = atan2(ry, rx) + a;
			t     = (xFixed_48_16) (angle * (65536. / (2*M_PI)));
			
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }

                    ++buffer;
                    rx += cx;
                    ry += cy;
                }
            } else {
                while (buffer < end) {
                    double x, y;
                    double angle;
		    
                    if (!mask || *mask++ & maskBits)
                    {
			xFixed_48_16  t;
			
			if (rz != 0) {
			    x = rx/rz;
			    y = ry/rz;
			} else {
			    x = y = 0.;
			}
			x -= pGradient->conical.center.x/65536.;
			y -= pGradient->conical.center.y/65536.;
			angle = atan2(y, x) + a;
			t     = (xFixed_48_16) (angle * (65536. / (2*M_PI)));
			
			WRITE(buffer, _gradient_walker_pixel (&walker, t));
		    }
		    
                    ++buffer;
                    rx += cx;
                    ry += cy;
                    rz += cz;
                }
            }
        }
    }
}
