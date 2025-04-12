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

/*
 * Scheduler Configuration
 * 
 * This header file defines configuration options for the Lightweight Task Scheduler (LTS).
 * Users can modify these settings to customize the behavior of the scheduler.
 */

#ifndef SCHEDULER_CONFIG_H
#define SCHEDULER_CONFIG_H

/* Define the maximum number of tasks that can be created in the scheduler. 
 * This value determines the size of the static task array in the scheduler context.
 * Adjust this value based on the application's requirements and memory constraints.
 */
#define MAX_TASKS 255

#endif /* SCHEDULER_CONFIG_H */
