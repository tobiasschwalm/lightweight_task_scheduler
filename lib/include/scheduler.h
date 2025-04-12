/*
 * MIT License
 * 
 * Copyright (c) 2025 Tobias Schwalm
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "platform_time.h"
#include "platform_types.h"

/* Include the configuration header */
#include "scheduler_config.h"

/* Definition of an error value used when task creation fails */
#define INVALID_TASK_ID MAX_TASKS

/* Type definition for task index */
typedef uint32_t task_id_t;

/* Function pointer type for tasks */
typedef void (*taskFunctionPtr_t)(void);

/* Enumeration for scheduler error codes */
typedef enum {
    SCHEDULER_SUCCESS = 0,
    SCHEDULER_ERROR_NULL_CONTEXT,
    SCHEDULER_ERROR_INVALID_TASK_ID,
    SCHEDULER_ERROR_MAX_TASKS_REACHED,
    SCHEDULER_ERROR_NO_TASKS
} SchedulerError;

/*
 * Structure representing a scheduler task.
 */
typedef struct Task 
{
    uint8_t id;                           /* Unique ID for the task */
    millisec_t cycle_ms;                  /* Cycle in milliseconds for task execution */
    millisec_t offset_ms;                 /* Offset in milliseconds for task execution */
    taskFunctionPtr_t task_function_ptr;  /* Pointer to the task function */
    millisec_t next_execution_time_ms;    /* Next execution time in milliseconds */
    uint8_t stopped;                      /* Flag to indicate if the task is stopped */    
} Task;

/*
 * Structure representing the scheduler context.
 */
typedef struct SchedulerContext
{
    task_id_t numTasks;                  /* Global variable to track the number of tasks created */
    Task tasks[MAX_TASKS];               /* Array to store all tasks */
    volatile uint8_t scheduler_stopped;  /* Flag to stop the scheduler loop */
} SchedulerContext;

/*
 * Creates a new task and adds it to the task list.
 * 
 * @param task_function_ptr Pointer to the function to be executed as a task.
 * @param cycle_ms The period in milliseconds at which the task should run.
 * @return The id of the created task, or INVALID_TASK_ID if the task could not be created.
 */
task_id_t create_task(SchedulerContext* ctx, const taskFunctionPtr_t task_function_ptr, const millisec_t cycle_ms, const millisec_t offset_ms);

/*
 * Removes a task from the task list.
 * 
 * @param ctx Pointer to the scheduler context.
 * @param task_id The id of the task to remove.
 * @return 1 if the task was removed successfully, 0 otherwise.
 */
SchedulerError remove_task(SchedulerContext* ctx, const task_id_t task_id);

/*
 * Starts a specific task that was previously stopped.
 * 
 * This function enables a task by its id, allowing it to be executed
 * in the scheduler loop. The task's next execution time is recalculated
 * based on the current time and its offset.
 * 
 * @param ctx Pointer to the scheduler context.
 * @param task_id The id of the task to start.
 * @return 1 if the task was successfully started, 0 otherwise.
 */
SchedulerError start_task(SchedulerContext* ctx, const task_id_t task_id);

/*
 * Stops a specific task from being executed.
 * 
 * This function disables a task by its id, preventing it from being executed
 * in the scheduler loop. The task remains in the task list but is effectively
 * paused.
 * 
 * @param ctx Pointer to the scheduler context.
 * @param task_id The id of the task to stop.
 * @return 1 if the task was successfully stopped, 0 otherwise.
 */
SchedulerError stop_task(SchedulerContext* ctx, const task_id_t task_id);

/*
 * Starts the scheduler loop to execute tasks based on their scheduled periods.
 * 
 * This function initializes the execution times for all tasks and enters a loop
 * to execute tasks when their scheduled time is reached. The loop continues until
 * the `scheduler_stopped` flag is set.
 */
SchedulerError start_scheduler(SchedulerContext* const ctx);

/*
 * Stops the scheduler loop.
 * 
 * This function sets the `scheduler_stopped` flag to stop the scheduler loop.
 * 
 * @param ctx Pointer to the scheduler context.
 * @return 1 if the task was successfully stopped, 0 otherwise.
 */
SchedulerError stop_scheduler(SchedulerContext* const ctx);

#endif /* SCHEDULER_H */