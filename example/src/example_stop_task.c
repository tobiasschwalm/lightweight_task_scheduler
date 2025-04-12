#include "scheduler.h"
#include "stdio.h"

SchedulerContext ctx = {0};
task_id_t periodic_task_id;

/* Task function */
void periodic_task(void)
{
    printf("Periodic task executed!\n");
}

/* One-time task to stop the periodic task */
void stop_task_once(void)
{
    printf("Stopping periodic task.\n");
    stop_task(&ctx, periodic_task_id); /* Stop the periodic task */
}

int main(void)
{
    /* Create the periodic task */
    periodic_task_id = create_task(&ctx, periodic_task, 1000, 0); /* Executes every 1000ms */

    /* Create a one-time task to stop the periodic task after 3000 ms */
    create_task(&ctx, stop_task_once, 0, 3000); /* Executes once after 3000 ms */

    /* Start the scheduler */
    start_scheduler(&ctx);

    return 0;
}
