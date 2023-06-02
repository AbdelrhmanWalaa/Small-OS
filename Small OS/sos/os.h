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
#define TICK_TIME		(1)			//tick time unit in millisecond


/************************************************************************/
/*						   type definitions					            */
/************************************************************************/
typedef void (*ptr_task_t) (void);

/************************************************************************/
/*						  TYPE DEFINITIONS					            */
/************************************************************************/
typedef enum{
	SOS_STATUS_INVALID,
	SOS_STATUS_SUCCESS
	}enu_system_status_t;

/************************************************************************/
/*						   function prototypes					        */
/************************************************************************/

/**
 * @brief                                           :   Function used to initialize SOS database
 * 
 * @param[in]   void								:   
 * 
 * @return      SOS_STATUS_SUCCESS                  :   in case of successful operation     
 *              SOS_STATUS_INVALID 					:   in case of the SOS is already initialized      
 */
enu_system_status_t sos_init (void);	

/**
 * @brief                                           :   Function used to reset the SOS database to invalid values
 * 
 * @param[in]   void								:   
 * 
 * @return      SOS_STATUS_SUCCESS                  :   in case of successful operation     
 *              SOS_STATUS_INVALID 					:   in case of the SOS is already initialized      
 */															
enu_system_status_t sos_deinit (void);

/**
 * @brief                                           :   Function used to create new task and add it to the SOS database
 * 
 * @param[in]   ptr_task							:	pointer to the task 
 * @param[in]   delay								:	delay act as shift to the task first start,if set to zero mean zero delay at the start  
 * @param[in]   period								:	period mean the periodicity of the task, if set to zero mean the task will be one shot task and then will be deleted
 * @param[out]  task_id								:   the task id in the SOS ,put it to [NULL] if you don't want to know the task id
 * 
 * @return      SOS_STATUS_SUCCESS                  :   in case of successful operation     
 *              SOS_STATUS_INVALID 					:   in case of the SOS database is full      
 */																	
enu_system_status_t sos_create_task (ptr_task_t  ptr_task,u16 delay,u16 period,u8* task_id);

/**
 * @brief                                           :   delete existing task from SOS database
 * 
 * @param[in]   task_id								:   the task id in the SOS
 * 
 * @return      SOS_STATUS_SUCCESS                  :   in case of successful operation     
 *              SOS_STATUS_INVALID 					:   in case of the task is not found     
 */			
enu_system_status_t sos_delete_task (u8 task_id);

/**
 * @brief                                           :   Function used to modify existing task parameters in the SOS database
 * 
 * @param[in]   ptr_task							:	pointer to the  new task 
 * @param[in]   delay								:	delay act as shift to the task first start,if set to zero mean zero delay at the start  
 * @param[in]   period								:	period mean the periodicity of the task, if set to zero mean the task will be one shot task and then will be deleted
 * @param[out]  task_id								:   the task id of the old task that need to modify in the SOS ,
 * 
 * @return      SOS_STATUS_SUCCESS                  :   in case of successful operation     
 *              SOS_STATUS_INVALID 					:   in case of the task is not found      
 */														
enu_system_status_t sos_modify_task (ptr_task_t  ptr_task,u16 delay,u16 period);

/**
 * @brief                                           :   Function used to run the scheduler
 * 
 * @param[in]   void								:   
 * 
 * @return      void								:       
 *                  
 */
					
void sos_run (void);

/**
 * @brief                                           :   Function used to stop the scheduler
 * 
 * @param[in]   void								:   
 * 
 * @return      void								:       
 *                  
 */																				
void sos_disable (void);																			


	
#endif /* OS_H_ */