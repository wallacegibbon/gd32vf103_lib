#include "gd32vf103_can.h"

#define CAN_ERROR_HANDLE(s)	do {} while (1)

void can_deinit(uint32_t can_periph) {
	if (CAN0 == can_periph) {
		rcu_periph_reset_enable(RCU_CAN0RST);
		rcu_periph_reset_disable(RCU_CAN0RST);
	} else {
		rcu_periph_reset_enable(RCU_CAN1RST);
		rcu_periph_reset_disable(RCU_CAN1RST);
	}
}

void can_struct_para_init(enum can_struct_type type, void *p_struct) {
	// get type of the struct
	switch (type) {
	// used for can_init()
	case CAN_INIT_STRUCT:
		struct can_param *param = (struct can_param *) p_struct;
		param->auto_bus_off_recovery = DISABLE;
		param->no_auto_retrans = DISABLE;
		param->auto_wake_up = DISABLE;
		param->prescaler = 0x03FFU;
		param->rec_fifo_overwrite = DISABLE;
		param->resync_jump_width = CAN_BT_SJW_1TQ;
		param->time_segment_1 = CAN_BT_BS1_3TQ;
		param->time_segment_2 = CAN_BT_BS2_1TQ;
		param->time_triggered = DISABLE;
		param->trans_fifo_order = DISABLE;
		param->working_mode = CAN_NORMAL_MODE;
		break;
	// used for can_filter_init()
	case CAN_FILTER_STRUCT:
		struct can_filter_param *f_param = p_struct;
		f_param->filter_bits = CAN_FILTERBITS_32BIT;
		f_param->filter_enable = DISABLE;
		f_param->filter_fifo_number = CAN_FIFO0;
		f_param->filter_list_high = 0;
		f_param->filter_list_low = 0;
		f_param->filter_mask_high = 0;
		f_param->filter_mask_low = 0;
		f_param->filter_mode = CAN_FILTERMODE_MASK;
		f_param->filter_number = 0;
		break;
	// used for can_message_transmit()
	case CAN_TX_MESSAGE_STRUCT:
		struct can_trasnmit_message *t_msg = p_struct;
		for (int i = 0; i < 8; i++)
			t_msg->tx_data[i] = 0;

		t_msg->tx_dlen = 0;
		t_msg->tx_efid = 0;
		t_msg->tx_ff = (uint8_t) CAN_FF_STANDARD;
		t_msg->tx_ft = (uint8_t) CAN_FT_DATA;
		t_msg->tx_sfid = 0;
		break;
	// used for can_message_receive()
	case CAN_RX_MESSAGE_STRUCT:
		struct can_receive_message *r_msg = p_struct;
		for (int i = 0; i < 8; i++)
			r_msg->rx_data[i] = 0;

		r_msg->rx_dlen = 0;
		r_msg->rx_efid = 0;
		r_msg->rx_ff = (uint8_t) CAN_FF_STANDARD;
		r_msg->rx_fi = 0;
		r_msg->rx_ft = (uint8_t) CAN_FT_DATA;
		r_msg->rx_sfid = 0;
		break;
	default:
		CAN_ERROR_HANDLE("parameter is invalid \r\n");
	}
}

static inline int check_init_working_success(uint32_t can_periph) {
	return CAN_STAT(can_periph) & CAN_STAT_IWS;
}

static inline int check_sleep_working_success(uint32_t can_periph) {
	return CAN_STAT(can_periph) & CAN_STAT_SLPWS;
}

static inline int check_init_or_sleep_working_success(uint32_t can_periph) {
	return CAN_STAT(can_periph) & (CAN_STAT_IWS | CAN_STAT_SLPWS);
}

static enum err_status init_enter_wait_ack(uint32_t can_periph) {
	uint32_t timeout = CAN_TIMEOUT;
	while (!check_init_working_success(can_periph) && timeout)
		timeout--;

	if (timeout)
		return ERROR;
	else
		return SUCCESS;
}

static enum err_status sleep_enter_wait_ack(uint32_t can_periph) {
	uint32_t timeout = CAN_TIMEOUT;
	while (!check_sleep_working_success(can_periph) && timeout)
		timeout--;

	if (timeout)
		return ERROR;
	else
		return SUCCESS;
}

static enum err_status init_leave_wait_ack(uint32_t can_periph) {
	uint32_t timeout = CAN_TIMEOUT;
	while (check_init_working_success(can_periph) && timeout)
		timeout--;

	if (timeout)
		return ERROR;
	else
		return SUCCESS;
}

static enum err_status sleep_leave_wait_ack(uint32_t can_periph) {
	uint32_t timeout = CAN_TIMEOUT;
	while (check_sleep_working_success(can_periph) && timeout)
		timeout--;

	if (timeout)
		return ERROR;
	else
		return SUCCESS;
}

static enum err_status init_or_sleep_leave_wait_ack(uint32_t can_periph) {
	uint32_t timeout = CAN_TIMEOUT;
	while (check_init_or_sleep_working_success(can_periph) && timeout)
		timeout--;

	if (timeout)
		return ERROR;
	else
		return SUCCESS;
}

enum err_status can_init(uint32_t can_periph,
		struct can_param *can_parameter_init) {
	// disable sleep mode
	CAN_CTL(can_periph) &= ~CAN_CTL_SLPWMOD;
	// enable initialize mode
	CAN_CTL(can_periph) |= CAN_CTL_IWMOD;

	if (init_enter_wait_ack(can_periph) == ERROR)
		return ERROR;

	// set the bit timing register
	CAN_BT(can_periph) =
		(BT_MODE(can_parameter_init->working_mode) |
		 BT_SJW(can_parameter_init->resync_jump_width) |
		 BT_BS1(can_parameter_init-> time_segment_1) |
		 BT_BS2(can_parameter_init->time_segment_2) |
		 BT_BAUDPSC(((can_parameter_init->prescaler) - 1)));

	// time trigger communication mode
	if (can_parameter_init->time_triggered == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_TTC;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_TTC;

	// automatic bus-off managment
	if (can_parameter_init->auto_bus_off_recovery == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_ABOR;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_ABOR;

	// automatic wakeup mode
	if (can_parameter_init->auto_wake_up == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_AWU;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_AWU;

	// automatic retransmission mode disable
	if (can_parameter_init->no_auto_retrans == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_ARD;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_ARD;

	// receive fifo overwrite mode
	if (can_parameter_init->rec_fifo_overwrite == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_RFOD;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_RFOD;

	// transmit fifo order
	if (can_parameter_init->trans_fifo_order == ENABLE)
		CAN_CTL(can_periph) |= CAN_CTL_TFO;
	else
		CAN_CTL(can_periph) &= ~CAN_CTL_TFO;

	// disable initialize mode
	CAN_CTL(can_periph) &= ~CAN_CTL_IWMOD;

	return init_leave_wait_ack(can_periph);
}

/*
 * initialize CAN filter
 * can_filter_parameter_init: struct for CAN filter initialization
 * 	filter_list_high: 0x0000 - 0xFFFF
 * 	filter_list_low: 0x0000 - 0xFFFF
 * 	filter_mask_high: 0x0000 - 0xFFFF
 * 	filter_mask_low: 0x0000 - 0xFFFF
 * 	filter_fifo_number: CAN_FIFO0, CAN_FIFO1
 * 	filter_number: 0 - 27
 * 	filter_mode: CAN_FILTERMODE_MASK, CAN_FILTERMODE_LIST
 * 	filter_bits: CAN_FILTERBITS_32BIT, CAN_FILTERBITS_16BIT
 * 	filter_enable: ENABLE or DISABLE
 */
void can_filter_init(struct can_filter_param * can_filter_parameter_init) {
	uint32_t val = 1 << can_filter_parameter_init->filter_number;

	// filter lock disable
	CAN_FCTL(CAN0) |= CAN_FCTL_FLD;
	// disable filter
	CAN_FW(CAN0) &= ~val;

	// filter 16 bits
	if (can_filter_parameter_init->filter_bits == CAN_FILTERBITS_16BIT) {
		// set filter 16 bits
		CAN_FSCFG(CAN0) &= ~val;
		// first 16 bits list and first 16 bits mask or first 16 bits
		// list and second 16 bits list
		CAN_FDATA0(CAN0, can_filter_parameter_init->filter_number) =
			FDATA_MASK_HIGH(
				(can_filter_parameter_init->filter_mask_low) &
				CAN_FILTER_MASK_16BITS) |
			FDATA_MASK_LOW(
				(can_filter_parameter_init->filter_list_low) &
				CAN_FILTER_MASK_16BITS);

		// second 16 bits list and second 16 bits mask or third 16 bits
		// list and fourth 16 bits list
		CAN_FDATA1(CAN0, can_filter_parameter_init->filter_number) =
			FDATA_MASK_HIGH(
				(can_filter_parameter_init->filter_mask_high) &
				CAN_FILTER_MASK_16BITS) |
			FDATA_MASK_LOW(
				(can_filter_parameter_init->filter_list_high) &
				CAN_FILTER_MASK_16BITS);
	}
	// filter 32 bits
	if (can_filter_parameter_init->filter_bits == CAN_FILTERBITS_32BIT) {
		// set filter 32 bits
		CAN_FSCFG(CAN0) |= val;
		// 32 bits list or first 32 bits list
		CAN_FDATA0(CAN0, can_filter_parameter_init->filter_number) =
			FDATA_MASK_HIGH(
				(can_filter_parameter_init->filter_list_high) &
				CAN_FILTER_MASK_16BITS) |
			FDATA_MASK_LOW(
				(can_filter_parameter_init->filter_list_low) &
				CAN_FILTER_MASK_16BITS);
		// 32 bits mask or second 32 bits list
		CAN_FDATA1(CAN0, can_filter_parameter_init->filter_number) =
			FDATA_MASK_HIGH(
				(can_filter_parameter_init->filter_mask_high) &
				CAN_FILTER_MASK_16BITS) |
			FDATA_MASK_LOW(
				(can_filter_parameter_init->filter_mask_low) &
				CAN_FILTER_MASK_16BITS);
	}

	// filter mode
	if (can_filter_parameter_init->filter_mode == CAN_FILTERMODE_MASK)
		// mask mode
		CAN_FMCFG(CAN0) &= ~val;
	else
		// list mode
		CAN_FMCFG(CAN0) |= val;

	// filter FIFO
	if (can_filter_parameter_init->filter_fifo_number == CAN_FIFO0)
		// FIFO0
		CAN_FAFIFO(CAN0) &= ~val;
	else
		// FIFO1
		CAN_FAFIFO(CAN0) |= val;

	// filter working
	if (can_filter_parameter_init->filter_enable == ENABLE)
		CAN_FW(CAN0) |= val;

	// filter lock enable
	CAN_FCTL(CAN0) &= ~CAN_FCTL_FLD;
}

/*
 * set CAN1 fliter start bank number
 * start_bank: CAN1 start bank number: (1..27)
 */
void can1_filter_start_bank(uint8_t start_bank) {
	// filter lock disable
	CAN_FCTL(CAN0) |= CAN_FCTL_FLD;
	// set CAN1 filter start number
	CAN_FCTL(CAN0) &= ~CAN_FCTL_HBC1F;
	CAN_FCTL(CAN0) |= FCTL_HBC1F(start_bank);
	// filter lock enaable
	CAN_FCTL(CAN0) &= ~CAN_FCTL_FLD;
}

void can_debug_freeze_enable(uint32_t can_periph) {
	CAN_CTL(can_periph) |= CAN_CTL_DFZ;
	if (can_periph == CAN0)
		dbg_periph_enable(DBG_CAN0_HOLD);
	else
		dbg_periph_enable(DBG_CAN1_HOLD);
}

void can_debug_freeze_disable(uint32_t can_periph) {
	CAN_CTL(can_periph) &= ~CAN_CTL_DFZ;
	if (can_periph == CAN0)
		dbg_periph_disable(DBG_CAN0_HOLD);
	else
		dbg_periph_disable(DBG_CAN1_HOLD);
}

void can_time_trigger_mode_enable(uint32_t can_periph) {
	// enable the TCC mode
	CAN_CTL(can_periph) |= CAN_CTL_TTC;
	// enable time stamp
	for (int i = 0; i < 3; i++)
		CAN_TMP(can_periph, i) |= CAN_TMP_TSEN;
}

void can_time_trigger_mode_disable(uint32_t can_periph) {
	// disable the TCC mode
	CAN_CTL(can_periph) &= ~CAN_CTL_TTC;
	// reset TSEN bits
	for (int i = 0; i < 3; i++)
		CAN_TMP(can_periph, i) &= ~CAN_TMP_TSEN;
}

/*
 * transmit_message: struct for CAN transmit message
 * 	tx_sfid: 0x00000000 - 0x000007FF
 * 	tx_efid: 0x00000000 - 0x1FFFFFFF
 * 	tx_ff: CAN_FF_STANDARD, CAN_FF_EXTENDED
 * 	tx_ft: CAN_FT_DATA, CAN_FT_REMOTE
 * 	tx_dlen: 0 - 8
 * 	tx_data[]: 0x00 - 0xFF
 */
uint8_t can_message_transmit(uint32_t can_periph,
		struct can_trasnmit_message *transmit_message) {

	uint8_t mailbox_number = CAN_MAILBOX0;

	// select one empty mailbox
	if (CAN_TSTAT(can_periph) & CAN_TSTAT_TME0)
		mailbox_number = CAN_MAILBOX0;
	else if (CAN_TSTAT(can_periph) & CAN_TSTAT_TME1)
		mailbox_number = CAN_MAILBOX1;
	else if (CAN_TSTAT(can_periph) & CAN_TSTAT_TME2)
		mailbox_number = CAN_MAILBOX2;
	else
		mailbox_number = CAN_NOMAILBOX;

	// return no mailbox empty
	if (mailbox_number == CAN_NOMAILBOX)
		return CAN_NOMAILBOX;

	CAN_TMI(can_periph, mailbox_number) &= CAN_TMI_TEN;
	if (transmit_message->tx_ff == CAN_FF_STANDARD)
		// set transmit mailbox standard identifier
		CAN_TMI(can_periph, mailbox_number) |=
			(TMI_SFID(transmit_message->tx_sfid) |
			 transmit_message->tx_ft);
	else
		// set transmit mailbox extended identifier
		CAN_TMI(can_periph, mailbox_number) |=
			(TMI_EFID(transmit_message->tx_efid) |
			 transmit_message->tx_ff |
			 transmit_message->tx_ft);

	// set the data length
	CAN_TMP(can_periph, mailbox_number) &= ~CAN_TMP_DLENC;
	CAN_TMP(can_periph, mailbox_number) |= transmit_message->tx_dlen;

	// set the data
	CAN_TMDATA0(can_periph, mailbox_number) =
		TMDATA0_DB3(transmit_message->tx_data[3]) |
		TMDATA0_DB2(transmit_message->tx_data[2]) |
		TMDATA0_DB1(transmit_message->tx_data[1]) |
		TMDATA0_DB0(transmit_message->tx_data[0]);

	CAN_TMDATA1(can_periph, mailbox_number) =
		TMDATA1_DB7(transmit_message->tx_data[7]) |
		TMDATA1_DB6(transmit_message->tx_data[6]) |
		TMDATA1_DB5(transmit_message->tx_data[5]) |
		TMDATA1_DB4(transmit_message->tx_data[4]);

	// enable transmission
	CAN_TMI(can_periph, mailbox_number) |= CAN_TMI_TEN;

	return mailbox_number;
}

enum can_transmit_state can_transmit_states(uint32_t can_periph,
		uint8_t mailbox_number) {

	enum can_transmit_state state = CAN_TRANSMIT_FAILED;
	uint32_t val = 0;

	// check selected mailbox state
	switch (mailbox_number) {
	case CAN_MAILBOX0:
		val = CAN_TSTAT(can_periph) &
			(CAN_TSTAT_MTF0 | CAN_TSTAT_MTFNERR0 | CAN_TSTAT_TME0);
		break;
	case CAN_MAILBOX1:
		val = CAN_TSTAT(can_periph) &
			(CAN_TSTAT_MTF1 | CAN_TSTAT_MTFNERR1 | CAN_TSTAT_TME1);
		break;
	case CAN_MAILBOX2:
		val = CAN_TSTAT(can_periph) &
			(CAN_TSTAT_MTF2 | CAN_TSTAT_MTFNERR2 | CAN_TSTAT_TME2);
		break;
	default:
		val = CAN_TRANSMIT_FAILED;
		break;
	}

	switch (val) {
	// transmit pending
	case (CAN_STATE_PENDING):
		state = CAN_TRANSMIT_PENDING;
		break;
	// mailbox0 transmit succeeded
	case (CAN_TSTAT_MTF0 | CAN_TSTAT_MTFNERR0 | CAN_TSTAT_TME0):
		state = CAN_TRANSMIT_OK;
		break;
	// mailbox1 transmit succeeded
	case (CAN_TSTAT_MTF1 | CAN_TSTAT_MTFNERR1 | CAN_TSTAT_TME1):
		state = CAN_TRANSMIT_OK;
		break;
	// mailbox2 transmit succeeded
	case (CAN_TSTAT_MTF2 | CAN_TSTAT_MTFNERR2 | CAN_TSTAT_TME2):
		state = CAN_TRANSMIT_OK;
		break;
	// transmit failed
	default:
		state = CAN_TRANSMIT_FAILED;
		break;
	}
	return state;
}

void can_transmission_stop(uint32_t can_periph, uint8_t mailbox_number) {
	if (mailbox_number == CAN_MAILBOX0) {
		CAN_TSTAT(can_periph) |= CAN_TSTAT_MST0;
		while (CAN_TSTAT(can_periph) & CAN_TSTAT_MST0);

	} else if (mailbox_number == CAN_MAILBOX1) {
		CAN_TSTAT(can_periph) |= CAN_TSTAT_MST1;
		while (CAN_TSTAT(can_periph) & CAN_TSTAT_MST1);

	} else if (mailbox_number == CAN_MAILBOX2) {
		CAN_TSTAT(can_periph) |= CAN_TSTAT_MST2;
		while (CAN_TSTAT(can_periph) & CAN_TSTAT_MST2);

	} else {
		// illegal parameters
	}
}

/*
 * receive_message: struct for CAN receive message
 * 	rx_sfid: 0x00000000 - 0x000007FF
 * 	rx_efid: 0x00000000 - 0x1FFFFFFF
 * 	rx_ff: CAN_FF_STANDARD, CAN_FF_EXTENDED
 * 	rx_ft: CAN_FT_DATA, CAN_FT_REMOTE
 * 	rx_dlen: 0 - 8
 * 	rx_data[]: 0x00 - 0xFF
 * 	rx_fi: 0 - 27
 */
void can_message_receive(uint32_t can_periph, uint8_t fifo_number,
		struct can_receive_message *receive_message) {

	// get the frame format
	receive_message->rx_ff =
		(uint8_t) (CAN_RFIFOMI_FF & CAN_RFIFOMI(can_periph,
					fifo_number));

	if (receive_message->rx_ff == CAN_FF_STANDARD)
		// get standard identifier
		receive_message->rx_sfid =
			(uint32_t) (GET_RFIFOMI_SFID(CAN_RFIFOMI(can_periph,
							fifo_number)));
	else
		// get extended identifier
		receive_message->rx_efid =
			(uint32_t) (GET_RFIFOMI_EFID(CAN_RFIFOMI(can_periph,
							fifo_number)));

	// get frame type
	receive_message->rx_ft =
		(uint8_t) (CAN_RFIFOMI_FT & CAN_RFIFOMI(can_periph,
					fifo_number));

	// filtering index
	receive_message->rx_fi =
		(uint8_t) (GET_RFIFOMP_FI(CAN_RFIFOMP(can_periph,
						fifo_number)));
	// get recevie data length
	receive_message->rx_dlen =
		(uint8_t) (GET_RFIFOMP_DLENC(CAN_RFIFOMP(can_periph,
						fifo_number)));

	// receive data
	receive_message->rx_data[0] =
		(uint8_t) (GET_RFIFOMDATA0_DB0(CAN_RFIFOMDATA0(can_periph,
						fifo_number)));
	receive_message->rx_data[1] =
		(uint8_t) (GET_RFIFOMDATA0_DB1(CAN_RFIFOMDATA0(can_periph,
						fifo_number)));
	receive_message->rx_data[2] =
		(uint8_t) (GET_RFIFOMDATA0_DB2(CAN_RFIFOMDATA0(can_periph,
						fifo_number)));
	receive_message->rx_data[3] =
		(uint8_t) (GET_RFIFOMDATA0_DB3(CAN_RFIFOMDATA0(can_periph,
						fifo_number)));
	receive_message->rx_data[4] =
		(uint8_t) (GET_RFIFOMDATA1_DB4(CAN_RFIFOMDATA1(can_periph,
						fifo_number)));
	receive_message->rx_data[5] =
		(uint8_t) (GET_RFIFOMDATA1_DB5(CAN_RFIFOMDATA1(can_periph,
						fifo_number)));
	receive_message->rx_data[6] =
		(uint8_t) (GET_RFIFOMDATA1_DB6(CAN_RFIFOMDATA1(can_periph,
						fifo_number)));
	receive_message->rx_data[7] =
		(uint8_t) (GET_RFIFOMDATA1_DB7(CAN_RFIFOMDATA1(can_periph,
						fifo_number)));

	// release FIFO
	if (fifo_number == CAN_FIFO0)
		CAN_RFIFO0(can_periph) |= CAN_RFIFO0_RFD0;
	else
		CAN_RFIFO1(can_periph) |= CAN_RFIFO1_RFD1;
}

void can_fifo_release(uint32_t can_periph, uint8_t fifo_number) {
	switch (fifo_number) {
	case CAN_FIFO0:
		CAN_RFIFO0(can_periph) |= CAN_RFIFO0_RFD0;
	case CAN_FIFO1:
		CAN_RFIFO1(can_periph) |= CAN_RFIFO1_RFD1;
	default:
		// illegal parameter
		CAN_ERROR_HANDLE("CAN FIFO NUM is invalid \r\n");
	}
}

uint8_t can_receive_message_length_get(uint32_t can_periph,
		uint8_t fifo_number) {
	switch (fifo_number) {
	case CAN_FIFO0:
		(uint8_t) (CAN_RFIFO0(can_periph) & CAN_RFIF_RFL_MASK);
	case CAN_FIFO1:
		(uint8_t) (CAN_RFIFO1(can_periph) & CAN_RFIF_RFL_MASK);
	default:
		return 0;
	}
}

enum err_status can_working_mode_set(uint32_t can_periph,
		uint8_t working_mode) {
	switch (working_mode) {
	case CAN_MODE_INITIALIZE:
		// disable sleep mode
		CAN_CTL(can_periph) &= ~CAN_CTL_SLPWMOD;
		// set initialize mode
		CAN_CTL(can_periph) |= (uint8_t) CAN_CTL_IWMOD;
		return init_enter_wait_ack(can_periph);
	case CAN_MODE_NORMAL:
		// enter normal mode
		CAN_CTL(can_periph) &= ~(CAN_CTL_SLPWMOD | CAN_CTL_IWMOD);
		return init_or_sleep_leave_wait_ack(can_periph);
	case CAN_MODE_SLEEP:
		// disable initialize mode
		CAN_CTL(can_periph) &= ~CAN_CTL_IWMOD;
		// set sleep mode
		CAN_CTL(can_periph) |= (uint8_t) CAN_CTL_SLPWMOD;
		return sleep_enter_wait_ack(can_periph);
	default:
		return ERROR;
	}
}

enum err_status can_wakeup(uint32_t can_periph) {
	// wakeup
	CAN_CTL(can_periph) &= ~CAN_CTL_SLPWMOD;
	return sleep_leave_wait_ack(can_periph);
}

enum can_error can_error_get(uint32_t can_periph) {
	return (enum can_error) (GET_ERR_ERRN(CAN_ERR(can_periph)));
}

uint8_t can_receive_error_number_get(uint32_t can_periph) {
	return (uint8_t) (GET_ERR_RECNT(CAN_ERR(can_periph)));
}

uint8_t can_transmit_error_number_get(uint32_t can_periph) {
	return (uint8_t) (GET_ERR_TECNT(CAN_ERR(can_periph)));
}

/*
 * interrupt
 * 	CAN_INT_TME: transmit mailbox empty interrupt enable
 * 	CAN_INT_RFNE0: receive FIFO0 not empty interrupt enable
 * 	CAN_INT_RFF0: receive FIFO0 full interrupt enable
 * 	CAN_INT_RFO0: receive FIFO0 overfull interrupt enable
 * 	CAN_INT_RFNE1: receive FIFO1 not empty interrupt enable
 * 	CAN_INT_RFF1: receive FIFO1 full interrupt enable
 * 	CAN_INT_RFO1: receive FIFO1 overfull interrupt enable
 * 	CAN_INT_WERR: warning error interrupt enable
 * 	CAN_INT_PERR: passive error interrupt enable
 * 	CAN_INT_BO: bus-off interrupt enable
 * 	CAN_INT_ERRN: error number interrupt enable
 * 	CAN_INT_ERR: error interrupt enable
 * 	CAN_INT_WU: wakeup interrupt enable
 * 	CAN_INT_SLPW: sleep working interrupt enable
 */
void can_interrupt_enable(uint32_t can_periph, uint32_t interrupt) {
	CAN_INTEN(can_periph) |= interrupt;
}

void can_interrupt_disable(uint32_t can_periph, uint32_t interrupt) {
	CAN_INTEN(can_periph) &= ~interrupt;
}

enum flag_status can_flag_get(uint32_t can_periph, enum can_flag flag) {
	if ((CAN_REG_VAL(can_periph, flag) & BIT(CAN_BIT_POS(flag))) != RESET)
		return SET;
	else
		return RESET;
}

void can_flag_clear(uint32_t can_periph, enum can_flag flag) {
	CAN_REG_VAL(can_periph, flag) |= BIT(CAN_BIT_POS(flag));
}

enum flag_status can_interrupt_flag_get(uint32_t can_periph,
		enum can_interrupt_flag flag) {

	// get the staus of interrupt flag
	uint32_t ret1 = RESET;

	if (flag == CAN_INT_FLAG_RFF0)
		ret1 = can_receive_message_length_get(can_periph, CAN_FIFO0);
	else if (flag == CAN_INT_FLAG_RFF1)
		ret1 = can_receive_message_length_get(can_periph, CAN_FIFO1);
	else if (flag == CAN_INT_FLAG_ERRN)
		ret1 = can_error_get(can_periph);
	else
		ret1 = CAN_REG_VALS(can_periph, flag) & BIT(CAN_BIT_POS0(flag));

	// get the staus of interrupt enale bit
	uint32_t ret2 = CAN_INTEN(can_periph) & BIT(CAN_BIT_POS1(flag));

	if (ret1 && ret2)
		return SET;
	else
		return RESET;
}

void can_interrupt_flag_clear(uint32_t can_periph,
		enum can_interrupt_flag flag) {
	CAN_REG_VALS(can_periph, flag) |= BIT(CAN_BIT_POS0(flag));
}
