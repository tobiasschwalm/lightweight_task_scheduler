#include "scheduler.h"
#include "stdio.h"

static SchedulerContext ctx = {0};

/* Task function */
void periodic_task(void)
{
    printf("Periodic task executed!\n");
}

/* Stop task */
void stop_scheduler_task(void)
{
    static int iteration_count = 0;

    if (++iteration_count >= 5)
    {
        printf("Stopping scheduler after 5 iterations.\n");
        stop_scheduler(&ctx);
    }
}

int main(void)
{
    create_task(&ctx, periodic_task, 1000, 0);
    create_task(&ctx, stop_scheduler_task, 1000, 0);
    start_scheduler(&ctx);
    
    return 0;
}
