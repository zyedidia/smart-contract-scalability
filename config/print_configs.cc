/**
 * Copyright 2023 Geoffrey Ramseyer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config/print_configs.h"
#include "config.h"

#include "config/static_constants.h"

#include <cstdio>
#include <thread>
#include <cinttypes>

namespace scs {

void
print_configs()
{
    std::printf("=========== scs configs  ==========\n");
    std::printf("USE_RPC      = %d\n", USE_RPC);
    std::printf("HW THREADS   = %u\n", std::thread::hardware_concurrency());
    std::printf("TLCACHE_SIZE = %" PRIu32 "\n", TLCACHE_SIZE);
}

} // namespace scs
