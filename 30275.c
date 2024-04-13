static int rgb2hsb(float *RGB)
{
    float HSB[3], v, diff;
    int i, j=0;

    v = 1.0;
    for (i=0;i<3;i++)
        HSB[i] = 0.0;
    for (i=0;i<3;i++) {
        if (RGB[i] > HSB[2]) {
            HSB[2] = RGB[i];
            j = i;
        }
        if (RGB[i] < v)
            v = RGB[i];
    }
    if (HSB[2] != 0) {
        diff = HSB[2] - v;
        HSB[1] = diff / HSB[2];
        switch (j) {
            case 0 : /* R == Brightness */
                /* diff can only be zero if r == br, so we need to make sure here we
                 * don't divide by zero
                 */
                if (diff)
                    HSB[0] = ((RGB[1] - RGB[2]) / (6.0 * diff)) + (RGB[2] > RGB[1] ? 1.0 : 0.0);
                else
                    HSB[0] = (RGB[1] - RGB[2]) + (RGB[2] > RGB[1] ? 1.0 : 0.0);
                break;
            case 1 : /* G == Brightness */
                HSB[0] = (1.0 / 3.0) + (RGB[2] - RGB[0]) / (6.0 * diff);
                break;
            case 2 : /* B == Brightness */
                HSB[0] = (2.0 / 3.0) + (RGB[0] - RGB[1]) / (6.0 * diff);
                break;
        }
    }
    for (i=0;i<3;i++) {
        if (HSB[i] < 0)
            HSB[i] = 0;
        if (RGB[i] > 1)
            HSB[i] = 1;
        RGB[i] = HSB[i];
    }
    return 0;
}
