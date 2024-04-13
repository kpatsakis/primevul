static storeProc storeProcForPicture (PicturePtr pict)
{
    switch(pict->format) {
    case PICT_a8r8g8b8: return fbStore_a8r8g8b8;
    case PICT_x8r8g8b8: return fbStore_x8r8g8b8;
    case PICT_a8b8g8r8: return fbStore_a8b8g8r8;
    case PICT_x8b8g8r8: return fbStore_x8b8g8r8;

        /* 24bpp formats */
    case PICT_r8g8b8: return fbStore_r8g8b8;
    case PICT_b8g8r8: return fbStore_b8g8r8;

        /* 16bpp formats */
    case PICT_r5g6b5: return fbStore_r5g6b5;
    case PICT_b5g6r5: return fbStore_b5g6r5;

    case PICT_a1r5g5b5: return fbStore_a1r5g5b5;
    case PICT_x1r5g5b5: return fbStore_x1r5g5b5;
    case PICT_a1b5g5r5: return fbStore_a1b5g5r5;
    case PICT_x1b5g5r5: return fbStore_x1b5g5r5;
    case PICT_a4r4g4b4: return fbStore_a4r4g4b4;
    case PICT_x4r4g4b4: return fbStore_x4r4g4b4;
    case PICT_a4b4g4r4: return fbStore_a4b4g4r4;
    case PICT_x4b4g4r4: return fbStore_x4b4g4r4;

        /* 8bpp formats */
    case PICT_a8: return  fbStore_a8;
    case PICT_r3g3b2: return fbStore_r3g3b2;
    case PICT_b2g3r3: return fbStore_b2g3r3;
    case PICT_a2r2g2b2: return fbStore_a2r2g2b2;
    case PICT_c8: return  fbStore_c8;
    case PICT_g8: return  fbStore_c8;
    case PICT_x4a4: return fbStore_x4a4;

        /* 4bpp formats */
    case PICT_a4: return  fbStore_a4;
    case PICT_r1g2b1: return fbStore_r1g2b1;
    case PICT_b1g2r1: return fbStore_b1g2r1;
    case PICT_a1r1g1b1: return fbStore_a1r1g1b1;
    case PICT_a1b1g1r1: return fbStore_a1b1g1r1;
    case PICT_c4: return  fbStore_c4;
    case PICT_g4: return  fbStore_c4;

        /* 1bpp formats */
    case PICT_a1: return  fbStore_a1;
    case PICT_g1: return  fbStore_g1;
    default:
        return NULL;
    }
}
