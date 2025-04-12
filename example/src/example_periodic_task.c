#include "scheduler.h"
#include "stdio.h"

/* Task function 1 */
void periodic_task_1(void)
{
    printf("Periodic task 1 executed!\n");
}

/* Task function 2 */
void periodic_task_2(void)
{
    printf("Periodic task 2 executed!\n");
}

int main(void)
{
    SchedulerContext ctx = {0};

     /* Executes every 1000 ms */
    create_task(&ctx, periodic_task_1, 1000, 0);

    /* Executes every 2000 ms and offset 500 ms */
    create_task(&ctx, periodic_task_2, 2000, 500); 

    /* Create and start the scheduler */
    start_scheduler(&ctx);

    return 0;
}
