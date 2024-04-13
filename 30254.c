static int hsb2rgb(float *HSB)
{
    float RGB[3], mn, md;
    int i;

    mn = (1.0 - HSB[1]) * HSB[2];
    md = 6.0 * HSB[1] * HSB[2];

    switch ((int)floor(6.0 * HSB[0])) {
        case 6:
            HSB[0] = (float)0;
            /* fall through */
        default: /* Shuts up compiler warning about RGB being uninited */
        case 0:
            RGB[0] = HSB[2];
            RGB[1] = mn + (HSB[0] * md);
            RGB[2] = mn;
            break;
        case 1:
            RGB[0] = mn + (md * ((1.0 / 3.0) - HSB[0]));
            RGB[1] = HSB[2];
            RGB[2] = mn;
            break;
        case 2:
            RGB[0] = mn;
            RGB[1] = HSB[2];
            RGB[2] = mn + ((HSB[0] - (1.0 / 3.0)) * md);
            break;
        case 3:
            RGB[0] = mn;
            RGB[1] = mn + (((2.0 / 3.0f) - HSB[0]) * md);
            RGB[2] = HSB[2];
            break;
        case 4:
            RGB[0] = mn + ((HSB[0] - (2.0 / 3.0)) * md);
            RGB[1] = mn;
            RGB[2] = HSB[2];
            break;
        case 5:
            RGB[0] = HSB[2];
            RGB[1] = mn;
            RGB[2] = mn + ((1.0 - HSB[0]) * md);
            break;
    }
    for (i=0;i<3;i++) {
        if (RGB[i] < 0)
            RGB[i] = 0;
        if (RGB[i] > 1)
            RGB[i] = 1;
        HSB[i] = RGB[i];
    }
    return 0;
}
