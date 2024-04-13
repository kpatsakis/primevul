test_unit_v2v_iter2(VALUE (* conv1)(VALUE),
		    VALUE (* conv2)(VALUE))
{
    if (!test_unit_v2v(INT2FIX(0), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(1), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(2), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(3), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(11), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(65535), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2FIX(1073741823), conv1, conv2))
	return 0;
    if (!test_unit_v2v(INT2NUM(1073741824), conv1, conv2))
	return 0;
    if (!test_unit_v2v(rb_rational_new2(INT2FIX(0), INT2FIX(1)), conv1, conv2))
	return 0;
    if (!test_unit_v2v(rb_rational_new2(INT2FIX(1), INT2FIX(1)), conv1, conv2))
	return 0;
    if (!test_unit_v2v(rb_rational_new2(INT2FIX(1), INT2FIX(2)), conv1, conv2))
	return 0;
    if (!test_unit_v2v(rb_rational_new2(INT2FIX(2), INT2FIX(3)), conv1, conv2))
	return 0;
    return 1;
}