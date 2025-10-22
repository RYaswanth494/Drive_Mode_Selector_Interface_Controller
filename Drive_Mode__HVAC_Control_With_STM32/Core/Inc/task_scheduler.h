/*
 * task_scheduler.h
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */

#ifndef INC_TASK_SCHEDULER_H_
#define INC_TASK_SCHEDULER_H_

#define max_tasks 20
typedef void (*TaskFunction)(void);


void Init_tasks(void);
int Register_task(uint32_t period_ms, TaskFunction task_fn);
int Schedule_task_once(uint32_t time_period, TaskFunction task_fn);
void Enable_task(int task_id);
void Disable_task(int task_id);
void Remove_task(int task_id);
void Run_all_tasks(void);

#endif /* INC_TASK_SCHEDULER_H_ */
