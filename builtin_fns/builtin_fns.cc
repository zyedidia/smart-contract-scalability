#include "builtin_fns/builtin_fns.h"

#include "wasm_api/wasm_api.h"

namespace scs
{

void
BuiltinFns::link_fns(wasm_api::WasmRuntime& runtime)
{
	/* -- env -- */

	runtime.template link_env<&BuiltinFns::memcmp>(
		"memcmp");
	runtime.template link_env<&BuiltinFns::memset>(
		"memset");
	runtime.template link_env<&BuiltinFns::memcpy>(
		"memcpy");

	/* -- logging functions -- */

	runtime.template link_fn<&BuiltinFns::scs_log>(
		"scs",
		"log");

	runtime.template link_fn<&BuiltinFns::scs_print_debug>(
		"scs",
		"print_debug");

	/* -- runtime functions -- */
	runtime.template link_fn<&BuiltinFns::scs_invoke>(
		"scs", 
		"invoke");

	runtime.template link_fn<&BuiltinFns::scs_return>(
		"scs",
		"return");

	runtime.template link_fn<&BuiltinFns::scs_get_calldata>(
		"scs",
		"get_calldata");

	runtime.template link_fn<&BuiltinFns::scs_get_msg_sender>(
		"scs",
		"get_msg_sender");

	runtime.template link_fn<&BuiltinFns::scs_get_self_addr>(
		"scs",
		"get_self");

	/* -- storage -- */
	runtime.template link_fn<&BuiltinFns::scs_raw_memory_set>(
		"scs",
		"raw_mem_set");
	runtime.template link_fn<&BuiltinFns::scs_raw_memory_get>(
		"scs",
		"raw_mem_get");
	runtime.template link_fn<&BuiltinFns::scs_raw_memory_get_len>(
		"scs",
		"raw_mem_len");

	/* -- deletions -- */
	runtime.template link_fn<&BuiltinFns::scs_delete_key_first>(
		"scs",
		"delete_key_first");

	runtime.template link_fn<&BuiltinFns::scs_delete_key_last>(
		"scs",
		"delete_key_last");

	/* -- nonnegative int64 -- */
	runtime.template link_fn<&BuiltinFns::scs_nonnegative_int64_set_add>(
		"scs",
		"nn_int64_set_add");

	runtime.template link_fn<&BuiltinFns::scs_nonnegative_int64_get>(
		"scs",
		"nn_int64_get");



}

} /* scs */
