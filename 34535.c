const uint8_t *ff_h263_find_resync_marker(const uint8_t *restrict p, const uint8_t * restrict end)
{
    assert(p < end);

    end-=2;
    p++;
    for(;p<end; p+=2){
        if(!*p){
            if     (!p[-1] && p[1]) return p - 1;
            else if(!p[ 1] && p[2]) return p;
        }
    }
    return end+2;
}
