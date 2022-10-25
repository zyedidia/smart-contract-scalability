%#include "xdr/types.h"

namespace scs
{

enum ObjectType
{
	RAW_MEMORY = 0,
	NONNEGATIVE_INT64 = 1,
	HASH_SET = 2,
};

const RAW_MEMORY_MAX_LEN = 512;

struct RawMemoryStorage
{
	opaque data<RAW_MEMORY_MAX_LEN>;
};

struct HashSet
{
	Hash hashes<>;
	uint32 max_size;
};

struct StorageObject
{
	union switch (ObjectType type)
	{
	case RAW_MEMORY:
		RawMemoryStorage raw_memory_storage;
	case NONNEGATIVE_INT64:
		int64 nonnegative_int64;
	case HASH_SET:
		HashSet hash_set;
	} body;

	uint64 escrowed_fee;
};

// in case of none:
// first ensure all are of same type -- if not, all fail
// then apply the single type semantics, using default starting value

} /* scs */
