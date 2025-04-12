#include "scheduler.h"
#include "platform_time.h"

#include <stdio.h>
#include <stdlib.h>

/* Custom test framework macros */
#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        printf("Assertion failed: %s == %s, at %s:%d\n", #expected, #actual, __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    }

#define RUN_TEST(test) \
    printf("Running %s...\n", #test); \
    setUp(); \
    test(); \
    tearDown(); \
    printf("%s passed.\n", #test);

/* Mock implementation of platform_time functions */
millisec_t mock_current_time = 0;

millisec_t current_time_ms(void)
{
    return mock_current_time;
}

void sleep_ms(const millisec_t time_ms)
{
    mock_current_time += time_ms;
    printf("Mock time progressed by %lu ms, current time: %lu ms\n",
         (unsigned long)time_ms, (unsigned long)mock_current_time);
}

/* Global variables for task execution tracking */
int task1_executions = 0;
int task2_executions = 0;

/* Test setup and teardown functions */
void setUp(void)
{
    mock_current_time = 0;
    task1_executions = 0;
    task2_executions = 0;
}

void tearDown(void)
{
    mock_current_time = 0;
    task1_executions = 0;
    task2_executions = 0;
}

/* Test cases */
void test_create_task(void)
{
    SchedulerContext ctx = {0};
    taskFunctionPtr_t mock_function = (taskFunctionPtr_t)1;

    /* Create a task and verify its properties */
    task_id_t task_id = create_task(&ctx, mock_function, 1000, 0);
    ASSERT_EQUAL(0, task_id);
    ASSERT_EQUAL(1, ctx.numTasks);
    ASSERT_EQUAL(1000, ctx.tasks[0].cycle_ms);
    ASSERT_EQUAL(mock_function, ctx.tasks[0].task_function_ptr);
}

void test_remove_task(void)
{
    SchedulerContext ctx = {0};
    taskFunctionPtr_t mock_function1 = (taskFunctionPtr_t)1;
    taskFunctionPtr_t mock_function2 = (taskFunctionPtr_t)2;

    /* Create two tasks */
    create_task(&ctx, mock_function1, 1000, 0);
    create_task(&ctx, mock_function2, 2000, 0);
    ASSERT_EQUAL(2, ctx.numTasks);

    /* Remove the first task and verify the remaining task */
    SchedulerError result = remove_task(&ctx, 0);
    ASSERT_EQUAL(SCHEDULER_SUCCESS, result);
    ASSERT_EQUAL(1, ctx.numTasks);
    ASSERT_EQUAL(mock_function2, ctx.tasks[0].task_function_ptr);
}

/* Mock task functions */
void mock_task1(void)
{
    printf("Mock Task 1 executed.\n");
    ++task1_executions;
}

void mock_task2(void)
{
    printf("Mock Task 2 executed.\n");
    ++task2_executions;
}

/* Global scheduler context and iteration counter for stop task */
static SchedulerContext* global_ctx = NULL;
static int iteration_counter = 0;

/* Task to stop the scheduler after a fixed number of iterations */
void stop_scheduler_task(void)
{
    iteration_counter++;
    printf("Iteration counter: %d\n", iteration_counter); 
    if (iteration_counter >= 5) /* Stop after 5 iterations */
    {
        printf("Stopping scheduler after 5 iterations.\n");
        stop_scheduler(global_ctx);
    }
}

void test_start_scheduler(void)
{
    SchedulerContext ctx = {0};
    global_ctx = &ctx; /* Set global context for stop task */

    /* Create tasks */
    create_task(&ctx, mock_task1, 1000, 0);
    create_task(&ctx, mock_task2, 2000, 0);
    create_task(&ctx, stop_scheduler_task, 1000, 0);

    /* Start the scheduler and verify it stops as expected */
    ctx.scheduler_stopped = 0;
    start_scheduler(&ctx);
    ASSERT_EQUAL(1, ctx.scheduler_stopped);
}

void test_scheduler_with_offsets(void)
{
    SchedulerContext ctx = {0};
    global_ctx = &ctx; /* Set global context for stop task */

    /* Reset execution counters */
    task1_executions = 0;
    task2_executions = 0;

    /* Create tasks with different cycles and offsets */
    create_task(&ctx, mock_task1, 1000, 200); /* Task 1: 1000 ms cycle, 200 ms offset */
    create_task(&ctx, mock_task2, 2000, 500); /* Task 2: 2000 ms cycle, 500 ms offset */
    create_task(&ctx, stop_scheduler_task, 1000, 0); /* Stop task: 1000 ms cycle, no offset */

    /* Adjust iteration counter to account for offsets */
    iteration_counter = 0;

    /* Start the scheduler */
    ctx.scheduler_stopped = 0;
    start_scheduler(&ctx);

    /* Verify task execution counts */
    ASSERT_EQUAL(4, task1_executions);
    ASSERT_EQUAL(2, task2_executions);
}

void test_task_with_cycle_zero_and_offset(void)
{
    SchedulerContext ctx = {0};
    global_ctx = &ctx;

    /* Reset execution counters */
    task1_executions = 0;

    /* Create a task with cycle time 0 and an offset 500 */
    create_task(&ctx, mock_task1, 0, 500);

    /* Start the scheduler */
    ctx.scheduler_stopped = 0;
    start_scheduler(&ctx);

    /* Verify task execution counts */
    ASSERT_EQUAL(1, task1_executions);
}

void test_scheduler_with_mixed_tasks(void)
{
    SchedulerContext ctx = {0};
    global_ctx = &ctx; /* Set global context for stop task */

    /* Reset execution counters */
    task1_executions = 0;
    task2_executions = 0;

    /* Create tasks with different cycles and offsets */
    create_task(&ctx, mock_task1, 1000, 200); /* Task 1: 1000 ms cycle, 200 ms offset */
    create_task(&ctx, mock_task2, 0, 500);    /* Task 2: Executes once after 500 ms offset */
    create_task(&ctx, stop_scheduler_task, 1000, 0); /* Stop task: 1000 ms cycle, no offset */

    /* Adjust iteration counter to account for offsets */
    iteration_counter = 0;

    /* Start the scheduler */
    ctx.scheduler_stopped = 0;
    start_scheduler(&ctx);

    /* Verify task execution counts */
    ASSERT_EQUAL(4, task1_executions);
    ASSERT_EQUAL(1, task2_executions);
}

void test_stop_scheduler(void)
{
    SchedulerContext ctx = {0};
    ctx.scheduler_stopped = 0;

    /* Stop the scheduler and verify its state */
    SchedulerError result = stop_scheduler(&ctx);
    ASSERT_EQUAL(SCHEDULER_SUCCESS, result);
    ASSERT_EQUAL(1, ctx.scheduler_stopped);
}

void test_start_task(void)
{
    SchedulerContext ctx = {0};
    create_task(&ctx, mock_task1, 1000, 0);

    /* Stop the task and verify */
    SchedulerError stop_result = stop_task(&ctx, 0);
    ASSERT_EQUAL(SCHEDULER_SUCCESS, stop_result);
    ASSERT_EQUAL(1, ctx.tasks[0].stopped);

    /* Start the task and verify */
    SchedulerError start_result = start_task(&ctx, 0);
    ASSERT_EQUAL(SCHEDULER_SUCCESS, start_result);
    ASSERT_EQUAL(0, ctx.tasks[0].stopped);
}

void test_stop_task(void)
{
    SchedulerContext ctx = {0};
    create_task(&ctx, mock_task1, 1000, 0);

    /* Stop the task and verify */
    SchedulerError result = stop_task(&ctx, 0);
    ASSERT_EQUAL(SCHEDULER_SUCCESS, result);
    ASSERT_EQUAL(1, ctx.tasks[0].stopped);
}

/* Main function to run the tests */
int main(void)
{
    RUN_TEST(test_create_task);
    RUN_TEST(test_remove_task);
    RUN_TEST(test_start_scheduler);
    RUN_TEST(test_stop_scheduler);
    RUN_TEST(test_scheduler_with_offsets);
    RUN_TEST(test_task_with_cycle_zero_and_offset);
    RUN_TEST(test_scheduler_with_mixed_tasks);
    RUN_TEST(test_start_task);
    RUN_TEST(test_stop_task);
    printf("All tests passed.\n");
    return EXIT_SUCCESS;
}
