test_unit_v2v_iter(VALUE (* conv1)(VALUE),
		   VALUE (* conv2)(VALUE))
{
    if (!test_unit_v2v_iter2(conv1, conv2))
	return 0;
    if (!test_unit_v2v_iter2(conv2, conv1))
	return 0;
    return 1;
}