#include "CAN.h"

/**
  *  can1发送数据函数
  *  标准ID
  *  
  */
void can1_transmit_data_sid(void)
{
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 01;		//ID值
	tx_message_struct.extended_id = 01;
	tx_message_struct.id_type = CAN_ID_STANDARD;		//标准ID类型
	tx_message_struct.frame_type = CAN_TFT_DATA;		//数据帧
	tx_message_struct.dlc = 8;
	tx_message_struct.data[0] = 0x10;		//数据
	tx_message_struct.data[1] = 0x11;
	tx_message_struct.data[2] = 0x12;
	tx_message_struct.data[3] = 0x13;
	tx_message_struct.data[4] = 0x14;
	tx_message_struct.data[5] = 0x15;
	tx_message_struct.data[6] = 0x16;
	tx_message_struct.data[7] = 0x17;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
	while(can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
	
}

/**
  *  can1发送数据函数
  *  扩展ID
  *  
  */
void can1_transmit_data_eid(void)
{
	uint8_t transmit_mailbox;
	can_tx_message_type tx_message_struct;
	tx_message_struct.standard_id = 02;
	tx_message_struct.extended_id = 0x18F5F200;		//ID值
	tx_message_struct.id_type = CAN_ID_EXTENDED;		//扩展ID
	tx_message_struct.frame_type = CAN_TFT_DATA;		//数据帧
	tx_message_struct.dlc = 8;							//数据长度
	tx_message_struct.data[0] = 0x18;		//数据
	tx_message_struct.data[1] = 0x19;
	tx_message_struct.data[2] = 0x1A;
	tx_message_struct.data[3] = 0x1B;
	tx_message_struct.data[4] = 0x1C;
	tx_message_struct.data[5] = 0x1D;
	tx_message_struct.data[6] = 0x1E;
	tx_message_struct.data[7] = 0x1F;
	transmit_mailbox = can_message_transmit(CAN1, &tx_message_struct);
	while(can_transmit_status_get(CAN1, (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
	
}





