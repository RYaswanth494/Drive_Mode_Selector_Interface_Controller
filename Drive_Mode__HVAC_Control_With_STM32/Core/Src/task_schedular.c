/*
 * task_schedular.c
 *
 *  Created on: Sep 3, 2025
 *      Author: MadaYaswanth
 */


#include"extern_file_declarations.h"
#define max_tasks 10
task_timer timer[max_tasks];

void Init_tasks(void){
	for(int i=0; i<max_tasks; i++){
		timer[i].period_ms = 0;
		timer[i].last_time_ms = 0;
		timer[i].enable = 0;
		timer[i].task_fn = 0;
		timer[i].one_time_execute = 0;
	}
}

int Register_task(uint32_t period_ms, TaskFunction task_fn){
	for(int i=0; i<max_tasks; i++){
		if(!timer[i].enable && timer[i].task_fn == 0){
			timer[i].enable =1;
			timer[i].last_time_ms = milli_sec_tick;
			timer[i].one_time_execute = 0;
			timer[i].period_ms = period_ms;
			timer[i].task_fn = task_fn;
			return i;
		}
	}
	return -1;
}

int Schedule_task_once(uint32_t time_period, TaskFunction task_fn){
	for(int i=0; i<max_tasks; i++){
		if(!timer[i].enable && timer[i].task_fn == 0){
			timer[i].enable =1;
			timer[i].last_time_ms = milli_sec_tick;
			timer[i].one_time_execute = 1;
			timer[i].period_ms = time_period;
			timer[i].task_fn = task_fn;
			return i;
		}
	}
	return -1;
}

void Run_all_tasks(void){
	for(int i=0; i<max_tasks; i++){
		if(timer[i].enable && timer[i].task_fn != 0){
			if(milli_sec_tick - timer[i].last_time_ms > timer[i].period_ms){
				timer[i].last_time_ms = milli_sec_tick;
				timer[i].task_fn();
				if(timer[i].one_time_execute==1){
					Remove_task(i);
				}
			}
		}
	}
}

void Remove_task(int task_id){
	if(task_id>=0 && task_id<max_tasks){
		timer[task_id].enable = 0;
		timer[task_id].last_time_ms = 0;
		timer[task_id].one_time_execute = 0;
		timer[task_id].period_ms = 0;
		timer[task_id].task_fn = 0;
	}
}

void Enable_task(int task_id){
	if(task_id>=0 && task_id<max_tasks){
		timer[task_id].enable = 1;
		timer[task_id].last_time_ms = milli_sec_tick;
	}
}

void Disable_task(int task_id){
	if(task_id>=0 && task_id<max_tasks){
		timer[task_id].enable = 0;
	}
}
