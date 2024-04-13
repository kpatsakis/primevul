static int rtrs_clt_rdma_cm_handler(struct rdma_cm_id *cm_id,
				     struct rdma_cm_event *ev)
{
	struct rtrs_clt_con *con = cm_id->context;
	struct rtrs_path *s = con->c.path;
	struct rtrs_clt_path *clt_path = to_clt_path(s);
	int cm_err = 0;

	switch (ev->event) {
	case RDMA_CM_EVENT_ADDR_RESOLVED:
		cm_err = rtrs_rdma_addr_resolved(con);
		break;
	case RDMA_CM_EVENT_ROUTE_RESOLVED:
		cm_err = rtrs_rdma_route_resolved(con);
		break;
	case RDMA_CM_EVENT_ESTABLISHED:
		cm_err = rtrs_rdma_conn_established(con, ev);
		if (!cm_err) {
			/*
			 * Report success and wake up. Here we abuse state_wq,
			 * i.e. wake up without state change, but we set cm_err.
			 */
			flag_success_on_conn(con);
			wake_up(&clt_path->state_wq);
			return 0;
		}
		break;
	case RDMA_CM_EVENT_REJECTED:
		cm_err = rtrs_rdma_conn_rejected(con, ev);
		break;
	case RDMA_CM_EVENT_DISCONNECTED:
		/* No message for disconnecting */
		cm_err = -ECONNRESET;
		break;
	case RDMA_CM_EVENT_CONNECT_ERROR:
	case RDMA_CM_EVENT_UNREACHABLE:
	case RDMA_CM_EVENT_ADDR_CHANGE:
	case RDMA_CM_EVENT_TIMEWAIT_EXIT:
		rtrs_wrn(s, "CM error (CM event: %s, err: %d)\n",
			 rdma_event_msg(ev->event), ev->status);
		cm_err = -ECONNRESET;
		break;
	case RDMA_CM_EVENT_ADDR_ERROR:
	case RDMA_CM_EVENT_ROUTE_ERROR:
		rtrs_wrn(s, "CM error (CM event: %s, err: %d)\n",
			 rdma_event_msg(ev->event), ev->status);
		cm_err = -EHOSTUNREACH;
		break;
	case RDMA_CM_EVENT_DEVICE_REMOVAL:
		/*
		 * Device removal is a special case.  Queue close and return 0.
		 */
		rtrs_clt_close_conns(clt_path, false);
		return 0;
	default:
		rtrs_err(s, "Unexpected RDMA CM error (CM event: %s, err: %d)\n",
			 rdma_event_msg(ev->event), ev->status);
		cm_err = -ECONNRESET;
		break;
	}

	if (cm_err) {
		/*
		 * cm error makes sense only on connection establishing,
		 * in other cases we rely on normal procedure of reconnecting.
		 */
		flag_error_on_conn(con, cm_err);
		rtrs_rdma_error_recovery(con);
	}

	return 0;
}