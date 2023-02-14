#pragma once

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

#include <cstdint>
#include <vector>

#include "xdr/types.h"
#include "xdr/transaction.h"

namespace scs {

struct MethodInvocation
{
	const Address addr;

	// Copying from eth design:
	// method names are 4 bytes (of valid ascii);
	// Here, public/private is denoted by method prefix
	// full method name is pub{XXXX}
	const uint32_t method_name;
	const std::vector<uint8_t> calldata;

	std::string 
	get_invocable_methodname() const;

	MethodInvocation(TransactionInvocation const& root_invocation);
	MethodInvocation(const Address& addr, uint32_t method, std::vector<uint8_t>&& calldata);
};

} /* scs */
