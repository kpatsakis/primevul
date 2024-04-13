void av_parser_close(AVCodecParserContext *s)
{
    if(s){
        if (s->parser->parser_close)
            s->parser->parser_close(s);
        av_free(s->priv_data);
        av_free(s);
    }
}
