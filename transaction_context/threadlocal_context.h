#pragma once

#include <concepts>
#include <memory>

#include "mtt/utils/threadlocal_cache.h"
#include "state_db/serial_delta_batch.h"

namespace scs
{

class ExecutionContext;
class WasmContext;

class ThreadlocalContextStore {
	inline static thread_local std::unique_ptr<ExecutionContext> ctx;

	inline static utils::ThreadlocalCache<SerialDeltaBatch> delta_batches;

	ThreadlocalContextStore() = delete;

public:
	static ExecutionContext& get_exec_ctx();
	static SerialDeltaBatch& get_delta_batch();
	
	template<typename WasmContext_T, typename ...Args>
	requires std::derived_from<WasmContext_T, WasmContext>
	static void make_ctx(Args&... args);

	static void post_block_clear();
};

} /* scs */
