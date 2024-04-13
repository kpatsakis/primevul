AVCodecParser* av_parser_next(AVCodecParser *p){
    if(p) return p->next;
    else  return av_first_parser;
}
