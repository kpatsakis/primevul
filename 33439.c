static fetchProc fetchProcForPicture (PicturePtr pict)
{
    switch(pict->format) {
    case PICT_a8r8g8b8: return fbFetch_a8r8g8b8;
    case PICT_x8r8g8b8: return fbFetch_x8r8g8b8;
    case PICT_a8b8g8r8: return fbFetch_a8b8g8r8;
    case PICT_x8b8g8r8: return fbFetch_x8b8g8r8;

        /* 24bpp formats */
    case PICT_r8g8b8: return fbFetch_r8g8b8;
    case PICT_b8g8r8: return fbFetch_b8g8r8;

        /* 16bpp formats */
    case PICT_r5g6b5: return fbFetch_r5g6b5;
    case PICT_b5g6r5: return fbFetch_b5g6r5;

    case PICT_a1r5g5b5: return fbFetch_a1r5g5b5;
    case PICT_x1r5g5b5: return fbFetch_x1r5g5b5;
    case PICT_a1b5g5r5: return fbFetch_a1b5g5r5;
    case PICT_x1b5g5r5: return fbFetch_x1b5g5r5;
    case PICT_a4r4g4b4: return fbFetch_a4r4g4b4;
    case PICT_x4r4g4b4: return fbFetch_x4r4g4b4;
    case PICT_a4b4g4r4: return fbFetch_a4b4g4r4;
    case PICT_x4b4g4r4: return fbFetch_x4b4g4r4;

        /* 8bpp formats */
    case PICT_a8: return  fbFetch_a8;
    case PICT_r3g3b2: return fbFetch_r3g3b2;
    case PICT_b2g3r3: return fbFetch_b2g3r3;
    case PICT_a2r2g2b2: return fbFetch_a2r2g2b2;
    case PICT_a2b2g2r2: return fbFetch_a2b2g2r2;
    case PICT_c8: return  fbFetch_c8;
    case PICT_g8: return  fbFetch_c8;
    case PICT_x4a4: return fbFetch_x4a4;

        /* 4bpp formats */
    case PICT_a4: return  fbFetch_a4;
    case PICT_r1g2b1: return fbFetch_r1g2b1;
    case PICT_b1g2r1: return fbFetch_b1g2r1;
    case PICT_a1r1g1b1: return fbFetch_a1r1g1b1;
    case PICT_a1b1g1r1: return fbFetch_a1b1g1r1;
    case PICT_c4: return  fbFetch_c4;
    case PICT_g4: return  fbFetch_c4;

        /* 1bpp formats */
    case PICT_a1: return  fbFetch_a1;
    case PICT_g1: return  fbFetch_g1;
    }

    return NULL;
}
