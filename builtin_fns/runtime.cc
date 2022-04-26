#include "builtin_fns/builtin_fns.h"

#include "transaction_context/execution_context.h"
#include "transaction_context/threadlocal_context.h"
#include "transaction_context/method_invocation.h"

namespace scs
{

void 
BuiltinFns::scs_return(int32_t offset, int32_t len)
{
	auto& tx_ctx = ThreadlocalContextStore::get_exec_ctx().get_transaction_context();

	tx_ctx.return_buf = tx_ctx.get_current_runtime()->template load_from_memory<std::vector<uint8_t>>(offset, len);
}

void 
BuiltinFns::scs_get_calldata(int32_t offset, int32_t len)
{
	auto& tx_ctx = ThreadlocalContextStore::get_exec_ctx().get_transaction_context();

	auto& calldata = tx_ctx.get_current_method_invocation().calldata;
	if (static_cast<uint32_t>(len) > calldata.size())
	{
		throw std::runtime_error("insufficient calldata");
	}

	tx_ctx.get_current_runtime() -> write_to_memory(calldata, offset, len);
}

void 
BuiltinFns::scs_invoke(
	int32_t addr_offset, 
	int32_t methodname, 
	int32_t calldata_offset, 
	int32_t calldata_len,
	int32_t return_offset,
	int32_t return_len)
{
	auto& tx_ctx = ThreadlocalContextStore::get_exec_ctx().get_transaction_context();

	auto& runtime = *tx_ctx.get_current_runtime();

	MethodInvocation invocation(
		runtime.template load_from_memory_to_const_size_buf<Address>(addr_offset),
		static_cast<uint32_t>(methodname),
		runtime.template load_from_memory<std::vector<uint8_t>>(calldata_offset, calldata_len));

	ThreadlocalContextStore::get_exec_ctx().invoke_subroutine(invocation);

	if (return_len > 0)
	{
		runtime.write_to_memory(tx_ctx.return_buf, return_offset, return_len);
	}

	tx_ctx.return_buf.clear();
}

void
BuiltinFns::scs_get_msg_sender(
	int32_t addr_offset
	/* addr_len = 32 */
	)
{
	auto& tx_ctx = ThreadlocalContextStore::get_exec_ctx().get_transaction_context();
	auto& runtime = *tx_ctx.get_current_runtime();

	auto const& invocation = tx_ctx.get_current_method_invocation();

	runtime.write_to_memory(invocation.addr, addr_offset, sizeof(invocation.addr));
}

} /* scs */
