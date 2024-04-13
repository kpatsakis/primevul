static bool rtrs_clt_change_state(struct rtrs_clt_path *clt_path,
				     enum rtrs_clt_state new_state)
{
	enum rtrs_clt_state old_state;
	bool changed = false;

	lockdep_assert_held(&clt_path->state_wq.lock);

	old_state = clt_path->state;
	switch (new_state) {
	case RTRS_CLT_CONNECTING:
		switch (old_state) {
		case RTRS_CLT_RECONNECTING:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_RECONNECTING:
		switch (old_state) {
		case RTRS_CLT_CONNECTED:
		case RTRS_CLT_CONNECTING_ERR:
		case RTRS_CLT_CLOSED:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_CONNECTED:
		switch (old_state) {
		case RTRS_CLT_CONNECTING:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_CONNECTING_ERR:
		switch (old_state) {
		case RTRS_CLT_CONNECTING:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_CLOSING:
		switch (old_state) {
		case RTRS_CLT_CONNECTING:
		case RTRS_CLT_CONNECTING_ERR:
		case RTRS_CLT_RECONNECTING:
		case RTRS_CLT_CONNECTED:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_CLOSED:
		switch (old_state) {
		case RTRS_CLT_CLOSING:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	case RTRS_CLT_DEAD:
		switch (old_state) {
		case RTRS_CLT_CLOSED:
			changed = true;
			fallthrough;
		default:
			break;
		}
		break;
	default:
		break;
	}
	if (changed) {
		clt_path->state = new_state;
		wake_up_locked(&clt_path->state_wq);
	}

	return changed;
}