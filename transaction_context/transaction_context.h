#pragma once

#include <cstdint>

#include "transaction_context/method_invocation.h"

#include "wasm_api/wasm_api.h"

#include "xdr/transaction.h"
#include "xdr/storage.h"

namespace scs {

class TransactionContext {

	DeltaPriority current_priority;

	std::vector<MethodInvocation> invocation_stack;
	std::vector<WasmRuntime*> runtime_stack;

public:

	const uint64_t gas_limit;
	uint64_t gas_used;

	const Address source_account;

	std::vector<uint8_t> return_buf;

	std::vector<std::vector<uint8_t>> logs;

	TransactionContext(uint64_t gas_limit, Address const& src, DeltaPriority d)
		: current_priority(d)
		, invocation_stack()
		, runtime_stack()
		, gas_limit(gas_limit)
		, gas_used(0)
		, source_account(src)
		, return_buf()
		{}

	DeltaPriority 
	get_next_priority();

	WasmRuntime*
	get_current_runtime();

	MethodInvocation& 
	get_current_method_invocation();

	void pop_invocation_stack();
	void push_invocation_stack(WasmRuntime* runtime, MethodInvocation const& invocation);

};

} /* namespace scs */
