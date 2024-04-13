initEmphClasses() {
	EmphasisClass *classes = malloc(10 * sizeof(EmphasisClass));
	int j;
	if (!classes) _lou_outOfMemory();
	for (j = 0; j < 10; j++) {
		classes[j] = 0x1 << (j + 1);
	}
	emphClasses = classes;
}
