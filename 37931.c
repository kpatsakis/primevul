void __init ima_init_policy(void)
{
	int i, entries;

	/* if !ima_use_tcb set entries = 0 so we load NO default rules */
	if (ima_use_tcb)
		entries = ARRAY_SIZE(default_rules);
	else
		entries = 0;

	for (i = 0; i < entries; i++)
		list_add_tail(&default_rules[i].list, &measure_default_rules);
	ima_measure = &measure_default_rules;
}
