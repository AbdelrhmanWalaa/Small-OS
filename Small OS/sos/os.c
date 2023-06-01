/*
 * os.c
 *
 * Created: 2023-05-29 11:55:33 AM
 *  Author: HAZEM-PC
 */ 
/************************************************************************/
/*						header files inclusion			                */
/************************************************************************/

#include "os.h"
#include "../MCAL/tmr1/tmr1_interface.h"

/************************************************************************/
/*						   type definitions					            */
/************************************************************************/
typedef enum
{
	WAIT,READY
}enu_task_states_t;

typedef enum
{
	NOT_INITIALIZE,INITIALIZE
}enu_sos_state_t;

typedef struct
{
	ptr_task_t			ptr_task;
	u16					initial_delay;
	u16					period;
	u16					priority;
	u8					task_id;
	enu_task_states_t	enu_task_states; 
}str_task_t;

/************************************************************************/
/*						   GLOBAL VARIABLES					            */
/************************************************************************/
str_task_t arr_str_task[SCH_MAX_TASK];
static enu_sos_state_t enu_sos_state = NOT_INITIALIZE;
static u8 u8_gl_flag=0;

/************************************************************************/
/*						  PRIVATE FUNCTIONS					            */
/************************************************************************/

static void sos_update(void);


/************************************************************************/
/*						  FUNTION DEFINITIONS 					        */
/************************************************************************/

enu_system_status_t sos_init (void)
{
	enu_system_status_t enu_system_status = SOS_STATUS_SUCCESS;
	u8 u8_l_index=0;
	str_tmr_configType str_tmr_config =
		{	.enu_tmr_mode		= NORMAL,
			.enu_tmr_clk		= CLK_STOP,
			.enu_tmr_intState	= EN_OVF
		};
	//initialize OS database by clear each index
	if(enu_sos_state == NOT_INITIALIZE)
	{
		for(u8_l_index = 0; u8_l_index < SCH_MAX_TASK ; u8_l_index++)
		{
			arr_str_task[u8_l_index].ptr_task		= NULL_PTR;
			arr_str_task[u8_l_index].initial_delay	= 0;
			arr_str_task[u8_l_index].period			= 0;
			arr_str_task[u8_l_index].task_id		= 0;
			arr_str_task[u8_l_index].priority		= -1;
			arr_str_task[u8_l_index].enu_task_states= WAIT;
		}
		enu_sos_state = INITIALIZE;
		//initialize timer according to tick time and set timer interrupts at required rate.
		tmr_Init(&str_tmr_config);
		tmr_setTimer(TICK_TIME);
		tmr_ovf_setCallback(sos_update);
	}
	else
	{
		enu_system_status = SOS_STATUS_INVALID;
	}
	return enu_system_status;
}
enu_system_status_t sos_deinit (void)
{
	enu_system_status_t enu_system_status = SOS_STATUS_SUCCESS;
	u8 u8_l_index=0;
	//reinitialize OS database by clear each index
	if(enu_sos_state == INITIALIZE)
	{
		for(u8_l_index = 0; u8_l_index < SCH_MAX_TASK ; u8_l_index++)
		{
			arr_str_task[u8_l_index].ptr_task		= NULL_PTR;
			arr_str_task[u8_l_index].initial_delay	= 0;
			arr_str_task[u8_l_index].period			= 0;
			arr_str_task[u8_l_index].task_id		= 0;
			arr_str_task[u8_l_index].priority		= -1;
			arr_str_task[u8_l_index].enu_task_states= WAIT;
		}
		enu_sos_state = NOT_INITIALIZE;
	}
	else
	{
		enu_system_status = SOS_STATUS_INVALID;
	}
	return enu_system_status;
}																	
enu_system_status_t sos_create_task (ptr_task_t  ptr_task,u16 delay,u16 period,u16 priority,u8* task_id)
{
	enu_system_status_t enu_system_status = SOS_STATUS_SUCCESS;
	u8 u8_l_index=0;
	// First find a gap in the array (if there is one)
	while((arr_str_task[u8_l_index].ptr_task != NULL_PTR) && (u8_l_index < SCH_MAX_TASK))
	{
		u8_l_index++;
	}
	// Have we reached the end of the list?
	if(u8_l_index == SCH_MAX_TASK)
	{
		// Task list is full
		enu_system_status = SOS_STATUS_INVALID;
	}
	else
	{
		// If we're here, there is a space in the task array
		arr_str_task[u8_l_index].ptr_task		= ptr_task;
		arr_str_task[u8_l_index].initial_delay	= delay;
		arr_str_task[u8_l_index].period			= period;
		arr_str_task[u8_l_index].priority		= priority;
		arr_str_task[u8_l_index].task_id		= u8_l_index;
		*task_id								= arr_str_task[u8_l_index].task_id;
	}
	return enu_system_status;	
}	
enu_system_status_t sos_delete_task (u8 task_id)
{
	enu_system_status_t enu_system_status = SOS_STATUS_SUCCESS;
	//check if there is task in that location
	if(arr_str_task[task_id].ptr_task != NULL_PTR)
	{
		//task found in that location
		arr_str_task[task_id].ptr_task			= NULL_PTR;
		arr_str_task[task_id].initial_delay		= 0;
		arr_str_task[task_id].period			= 0;
		arr_str_task[task_id].task_id			= 0;
		arr_str_task[task_id].priority			= -1;
		arr_str_task[task_id].enu_task_states	= WAIT;
	}
	else
	{
		//no task found in that location
		enu_system_status = SOS_STATUS_INVALID;
	}
	return enu_system_status; 
}								
enu_system_status_t sos_modify_task (ptr_task_t  ptr_task,u16 delay,u16 period,u16 priority)
{
	enu_system_status_t enu_system_status = SOS_STATUS_SUCCESS;
	u8 u8_l_index=0;
	if(ptr_task != NULL_PTR)
	{
		for(u8_l_index = 0; u8_l_index < SCH_MAX_TASK ; u8_l_index++)
		{
			if(arr_str_task[u8_l_index].ptr_task == ptr_task)
			{
				arr_str_task[u8_l_index].initial_delay	=delay;
				arr_str_task[u8_l_index].period			=period;
				arr_str_task[u8_l_index].priority		=priority;
				break;
			}
		}
		if(u8_l_index == SCH_MAX_TASK)
		{
			enu_system_status = SOS_STATUS_INVALID;
		}
	}
	else
	{
		enu_system_status = SOS_STATUS_INVALID;
	}
	return enu_system_status;
}

	
void sos_run (void)
{
	u8 u8_l_index=0;
	for(u8_l_index = 0; u8_l_index < SCH_MAX_TASK ; u8_l_index++)
	{
		if(arr_str_task[u8_l_index].enu_task_states == READY)
		{
			(*arr_str_task[u8_l_index].ptr_task)();		//run the task
			arr_str_task[u8_l_index].enu_task_states = WAIT;
			if(arr_str_task[u8_l_index].period == 0)	//one shot task
			{
				sos_delete_task(u8_l_index);			//remove the task from OS database
			}
			break;
		}
	}
	if(u8_gl_flag == 1)
	{
		u8_gl_flag = 0;
		for(u8_l_index = 0; u8_l_index < SCH_MAX_TASK ; u8_l_index++)
		{
			if(arr_str_task[u8_l_index].ptr_task != NULL_PTR)
			{
				if(arr_str_task[u8_l_index].initial_delay == 0)
				{
					arr_str_task[u8_l_index].enu_task_states = READY;
					if(arr_str_task[u8_l_index].period > 0)
					{
						//Schedule periodic tasks to run again
						arr_str_task[u8_l_index].initial_delay = arr_str_task[u8_l_index].period;
					}
				}
				else
				{
					arr_str_task[u8_l_index].initial_delay--;
				}
			}
		}
	}
}																		
void sos_disable (void)
{
	//stop timer interrupt
	tmr_Stop();
}


static void sos_update(void)
{
	u8_gl_flag = 1;
}


