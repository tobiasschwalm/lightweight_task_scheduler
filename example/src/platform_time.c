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

#include "platform_time.h"
#include <time.h>

/*
 * Placeholder implementation for platform-specific time functions.
 * Replace this with the actual implementation for your target platform.
 */

/*
 * Get the current time in milliseconds.
 */
millisec_t current_time_ms(void)
{
    return (millisec_t)((clock() * 1000) / CLOCKS_PER_SEC);
}

/*
 * Sleep for a specified amount of time in milliseconds.
 */
void sleep_ms(const millisec_t time_ms)
{
    clock_t start_time = clock();
    clock_t wait_ticks = (clock_t)((time_ms * CLOCKS_PER_SEC) / 1000);
    while ((clock() - start_time) < wait_ticks)
    {
        /* Busy-wait loop for delay */
    }
}
