/*
 * os.h
 *
 * Created: 2023-05-29 11:55:17 AM
 *  Author: HAZEM-PC
 */ 


#ifndef OS_H_
#define OS_H_

/************************************************************************/
/*						header files inclusion			                */
/************************************************************************/
#include "../utilities/Std_Types.h"


/************************************************************************/
/*						   MACRO definitions					        */
/************************************************************************/
#define SCH_MAX_TASK	(10)		//size of OS database
#define TICK_TIME		(1)		//tick time unit in millisecond


/************************************************************************/
/*						   type definitions					            */
/************************************************************************/
typedef void (*ptr_task_t) (void);

/************************************************************************/
/*						  PRIVATE FUNCTIONS					            */
/************************************************************************/

typedef enum{
	SOS_STATUS_INVALID,
	SOS_STATUS_SUCCESS
	}enu_system_status_t;

/************************************************************************/
/*						   function prototypes					        */
/************************************************************************/
enu_system_status_t sos_init (void);																		/*initialize SOS database*/
enu_system_status_t sos_deinit (void);																		/*reset the SOS database to invalid values*/
enu_system_status_t sos_create_task (ptr_task_t  ptr_task,u16 delay,u16 period,u8* task_id);				/*create new task and add it to the SOS database*/
enu_system_status_t sos_delete_task (u8 task_id);															/*delete existing task from SOS database*/
enu_system_status_t sos_modify_task (ptr_task_t  ptr_task,u16 delay,u16 period);							/*modify existing task parameters in the SOS database*/
void sos_run (void);																						/*run the scheduler*/
void sos_disable (void);																					/*stop the scheduler*/


	
#endif /* OS_H_ */