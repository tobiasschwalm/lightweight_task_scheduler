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

#ifndef PLATFORM_TIME_H
#define PLATFORM_TIME_H

#include "platform_types.h"

/* Type definition for time in milliseconds */
typedef uint64_t millisec_t;

/* Maximum value for millisec_t */
#define MAX_MILLISEC ((millisec_t)-1) 

/*
 * Get the current time in milliseconds.
 * 
 * This function retrieves the current system time in milliseconds
 * since an arbitrary epoch (e.g., system start or UNIX epoch).
 * 
 * Platforms must provide an implementation of this function.
 * 
 * @return millisec_t The current time in milliseconds.
 */
millisec_t current_time_ms(void);

/*
 * Sleep for a specified amount of time in milliseconds.
 * 
 * This function pauses the execution of the program for the specified
 * duration in milliseconds. Platforms must provide an implementation.
 * 
 * @param time_ms The duration to sleep in milliseconds.
 */
void sleep_ms(const millisec_t time_ms);

#endif /* PLATFORM_TIME_H */
