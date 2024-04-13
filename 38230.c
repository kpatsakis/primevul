static irqreturn_t au1200fb_handle_irq(int irq, void* dev_id)
{
	/* Nothing to do for now, just clear any pending interrupt */
	lcd->intstatus = lcd->intstatus;
	au_sync();

	return IRQ_HANDLED;
}
