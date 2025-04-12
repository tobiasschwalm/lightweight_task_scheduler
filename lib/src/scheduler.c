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

#include "scheduler.h"
#include "platform_time.h"

task_id_t create_task(SchedulerContext* ctx, const taskFunctionPtr_t task_function_ptr, const millisec_t cycle_ms, const millisec_t offset_ms) 
{
    if (ctx == NULL || task_function_ptr == NULL || ctx->numTasks >= MAX_TASKS) return MAX_TASKS; 
    ctx->tasks[ctx->numTasks].id = ctx->numTasks;
    ctx->tasks[ctx->numTasks].cycle_ms = cycle_ms;
    ctx->tasks[ctx->numTasks].offset_ms = offset_ms;
    ctx->tasks[ctx->numTasks].task_function_ptr = task_function_ptr;
    ctx->tasks[ctx->numTasks].next_execution_time_ms = 0;
    ctx->tasks[ctx->numTasks].stopped = 0;
    return ctx->numTasks++;
}

SchedulerError remove_task(SchedulerContext* ctx, const task_id_t task_id)
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    if (task_id >= ctx->numTasks) return SCHEDULER_ERROR_INVALID_TASK_ID;
    /* Shift tasks to fill the gap */
    for (task_id_t i = task_id; i < ctx->numTasks - 1; ++i)
    {
        ctx->tasks[i] = ctx->tasks[i + 1];
    }
    ctx->numTasks--;
    return SCHEDULER_SUCCESS;
}

/*
 * Sets the inital execution times for a all tasks.
 * 
 * @param ctx Pointer to the scheduler context.
 * @return 1 if the execution times were successfully set, 0 otherwise.
 */
SchedulerError set_initial_execution_times(SchedulerContext* const ctx) 
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    if (ctx->numTasks == 0) return SCHEDULER_ERROR_NO_TASKS;
    const millisec_t current_time = current_time_ms();
    for (task_id_t task_id = 0; task_id < ctx->numTasks; ++task_id)
    {
        ctx->tasks[task_id].next_execution_time_ms = current_time + ctx->tasks[task_id].offset_ms;
        if (ctx->tasks[task_id].next_execution_time_ms < current_time)
        {
            /* Handle overflow or invalid offset */
            ctx->tasks[task_id].next_execution_time_ms = current_time;
        }
    }
    return SCHEDULER_SUCCESS;
}

/*
 * Checks which task is next for execution and runs this.
 * 
 * @param ctx Pointer to the scheduler context.
 * @return earliest wakeup time for next iteration of execution, MAX_MILLISEC otherwise in case of failure.
 */
millisec_t execute_tasks(SchedulerContext* const ctx) 
{
    if (ctx == NULL || ctx->numTasks == 0) return MAX_MILLISEC;
    const millisec_t current_time = current_time_ms();
    millisec_t earliest_wakeup_time_ms = MAX_MILLISEC;
    for (task_id_t task_id = 0; task_id < ctx->numTasks; ++task_id)
    {
        if (ctx->tasks[task_id].task_function_ptr == NULL) continue;
        if ((current_time >= ctx->tasks[task_id].next_execution_time_ms) && (ctx->tasks[task_id].stopped == 0))
        {
            ctx->tasks[task_id].task_function_ptr();
            /* If the task has a cycle of 0, it should be stopped after its first execution */
            if (ctx->tasks[task_id].cycle_ms == 0)
            {
                ctx->tasks[task_id].stopped = 1; /* Ensure the task is marked as stopped */
                ctx->tasks[task_id].next_execution_time_ms = MAX_MILLISEC; /* Prevent further execution */
            }
            else
            {
                /* If the task has a cycle, set the next execution time */
                ctx->tasks[task_id].next_execution_time_ms = current_time + ctx->tasks[task_id].cycle_ms;
            }
        }
        /* Update earliest wakeup time */
        if (ctx->tasks[task_id].next_execution_time_ms < earliest_wakeup_time_ms)
        {
            earliest_wakeup_time_ms = ctx->tasks[task_id].next_execution_time_ms;
        }
    }
    return earliest_wakeup_time_ms;
}

SchedulerError start_task(SchedulerContext* const ctx, task_id_t task_id)
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    if (task_id >= ctx->numTasks) return SCHEDULER_ERROR_INVALID_TASK_ID;
    ctx->tasks[task_id].next_execution_time_ms = current_time_ms() + ctx->tasks[task_id].offset_ms;
    ctx->tasks[task_id].stopped = 0;
    return SCHEDULER_SUCCESS;
}

SchedulerError stop_task(SchedulerContext* const ctx, task_id_t task_id)
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    if (task_id >= ctx->numTasks) return SCHEDULER_ERROR_INVALID_TASK_ID;
    ctx->tasks[task_id].stopped = 1;
    return SCHEDULER_SUCCESS;
}

SchedulerError start_scheduler(SchedulerContext* const ctx)
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    if (ctx->numTasks == 0) return SCHEDULER_ERROR_NO_TASKS;
    ctx->scheduler_stopped = 0;
    set_initial_execution_times(ctx);
    while (ctx->scheduler_stopped == 0)
    {
        const millisec_t earliest_wakeup_time_ms = execute_tasks(ctx);
        const millisec_t current_time = current_time_ms();
        /* Check if no tasks are scheduled for execution */
        if (earliest_wakeup_time_ms == MAX_MILLISEC)
        {
            stop_scheduler(ctx);
            break;
        }
        /* Sleep until the next task is ready */
        if (earliest_wakeup_time_ms > current_time)
        {
            sleep_ms(earliest_wakeup_time_ms - current_time);
        }
    }
    return SCHEDULER_SUCCESS;
}

SchedulerError stop_scheduler(SchedulerContext* const ctx)
{
    if (ctx == NULL) return SCHEDULER_ERROR_NULL_CONTEXT;
    ctx->scheduler_stopped = 1;
    return SCHEDULER_SUCCESS;
}