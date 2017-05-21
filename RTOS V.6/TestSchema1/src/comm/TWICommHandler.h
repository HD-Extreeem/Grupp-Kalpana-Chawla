/*
 * TWICommHandler.h
 *
 * Created: 2017-05-12 09:59:23
 * Author: Sebastian Börebäck
 * Version 1.1
 * 
 * Modified by Jonas Eiselt (changed variable names and formatted document)
 */ 

#ifndef TWICOMMHANDLER_H_
#define TWICOMMHANDLER_H_

#define TWI_STANDARD_MODE_SPEED 100000U			// Standard clock speed of TWI
#define TWI_FAST_MODE_SPEED		400000U			// Fast speed of TWI (more chance for NACK and errors)

#define SLAVE_ADDR_NAV	0x03					// TWI slave address for navigation
#define SLAVE_ADDR_ARM	0x02					// TWI slave address for navigation

#define TWI_SPEED	TWI_STANDARD_MODE_SPEED
#define TWI_PORT	TWI0						// Set the TWI port to be used

#define TWI_NO_DATA 0x78

typedef struct 
{
	/* Distances are in cm and angles are in degrees */
	uint8_t box_distance;
	uint8_t box_angle;
	uint8_t object_distance;
	uint8_t object_angle;
	uint8_t collect_all;						// False if robot can only carry one object at a time
	uint8_t has_data;							// Just to check if the struct is set
} Arm_Info;

/* TWI states */
typedef enum  
{
	TWI_CMD_ARM_INIT			= 0x20,
	TWI_CMD_DROP_OFF_START		= 0x21,
	TWI_CMD_PICK_UP_START		= 0x22,
	TWI_CMD_DROP_OFF_STATUS		= 0x23,
	TWI_CMD_PICK_UP_STATUS		= 0x24,
	TWI_CMD_ERROR				= 0x25,
	TWI_CMD_FIND_OBJECT			= 0x26,
	TWI_CMD_FIND_OBJECT_STATUS	= 0x27
} TWI_CMD;

typedef enum  
{
	TWI_CMD_ARM_REQ_BOX_INFO		= 2,
	TWI_CMD_ARM_REQ_OBJ_INFO		= 3,
	TWI_CMD_ARM_REQ_COLLECT_INFO	= 4
} TWI_CMD_Init_Req;

typedef enum
{
	OBJECT_NOT_FOUND	= 2,
	OBJECT_FOUND		= 3,
	ARLO_ROTATE_LEFT	= 4,
	ARLO_ROTATE_RIGHT	= 5,
	ARLO_FORWARD		= 6,
	ARLO_BACKWARD		= 7,
	ARLO_READY			= 8
} Find_Object_Status;

typedef enum 
{
	PICK_UP_DONE		= 2,
	PICK_UP_FORWARD		= 3,
	PICK_UP_BACKWARD	= 4,
	PICK_UP_RUNNING		= 5,
	PICK_UP_FAILED		= 6,
	PICK_UP_DONE_DRIVE	= 7,
	PICK_UP_IDLE		= 8,
	PICK_UP_ROTATE_LEFT = 9,
	PICK_UP_ROTATE_RIGHT= 10
} Pick_Up_Status;

typedef enum 
{
	DROP_OFF_DONE		= 2,
	DROP_OFF_RUNNING	= 3,
	DROP_OFF_FAILED		= 4,
	DROP_OFF_IDLE		= 5
} Drop_Off_Status;

typedef enum 
{
	SOCK	= 2,
	CUBE	= 3,
	GLASS	= 4,
	OBJECT	= 5
} Object;

#endif /* TWICOMMHANDLER_H_ */