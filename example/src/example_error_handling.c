#include "scheduler.h"
#include "stdio.h"

/* Task function */
void example_task(void)
{
    printf("Example task executed!\n");
}

int main(void)
{
    SchedulerContext ctx = {0};

    /* Attempt to create a task */
    task_id_t task_id = create_task(&ctx, example_task, 1000, 0); /* Executes every 1000 ms */
    if (task_id == INVALID_TASK_ID)
    {
        fprintf(stderr, "Error: Failed to create task. Maximum task limit reached.\n");
        return 1; /* Exit with error */
    }

    /* Attempt to start the scheduler */
    SchedulerError scheduler_result = start_scheduler(&ctx);
    switch (scheduler_result)
    {
        case SCHEDULER_SUCCESS:
            break; /* Scheduler started successfully */

        case SCHEDULER_ERROR_NULL_CONTEXT:
            fprintf(stderr, "Error: Scheduler context is null.\n");
            return 1; /* Exit with error */

        case SCHEDULER_ERROR_NO_TASKS:
            fprintf(stderr, "Error: No tasks available to schedule.\n");
            return 1; /* Exit with error */

        default:
            fprintf(stderr, "Error: Unknown error occurred while starting the scheduler.\n");
            return 1; /* Exit with error */
    }

    return 0;
}
