pdf14_debug_mask_stack_state(pdf14_ctx *ctx)
{
    if_debug1m('v', ctx->memory, "[v]ctx_maskstack, %x\n", ctx->mask_stack);
    if (ctx->mask_stack != NULL) {
        dump_mask_stack(ctx->mask_stack);
    }
    if_debug1m('v', ctx->memory, "[v]ctx_stack, %x\n", ctx->stack);
    if (ctx->stack != NULL) {
        if_debug1m('v', ctx->memory, "[v]ctx_stack_maskstack, %x\n", ctx->stack->mask_stack);
        if (ctx->stack->mask_stack != NULL) {
            dump_mask_stack(ctx->stack->mask_stack);
        }
    }
}
