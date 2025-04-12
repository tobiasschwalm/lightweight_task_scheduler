#include "scheduler.h"
#include "stdio.h"

/* Task function */
void one_time_task(void)
{
    printf("One-time task executed!\n");
}

int main(void)
{
    SchedulerContext ctx = {0};

    /* Create a one-time task */
    create_task(&ctx, one_time_task, 0, 2000); /* Executes once after 2000 ms */
    start_scheduler(&ctx);

    return 0;
}
