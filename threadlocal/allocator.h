#pragma once

#include "config/static_constants.h"
#include <cstdint>
#include <memory>
#include <array>

#include <utils/threadlocal_cache.h>

namespace scs
{

// the "0" elt is reserved, never valid
template<typename T>
struct ThreadlocalBlockAllocator
{
	constexpr static size_t alloc_size = 1'000'000;
	uint32_t offset;

	// allocating this directly causes 'ld: LC_SEGMENT filesize too large file'
	// at link time (with -O3 -flto)
	std::unique_ptr<std::array<T, alloc_size>> buffer;

	const uint32_t buffer_idx = (utils::ThreadlocalIdentifier::get()) << 24;

	ThreadlocalBlockAllocator()
		: offset(0)
		, buffer(std::make_unique<std::array<T, alloc_size>>())
		{}

	uint32_t 
	allocate(T&& h)
	{
		offset++;
		(*buffer)[offset - 1] = std::move(h);
		uint32_t out = offset;
		if (offset - 1 >= alloc_size)
		{
			throw std::runtime_error("alloc max reached");
		}
		return out + buffer_idx;
	}

	T const& get(uint32_t idx) const
	{
		return (*buffer)[idx - 1];
	}

	void reset() {
		offset = 0;
	}
};

template<typename T>
struct BlockAllocator
{
	utils::ThreadlocalCache<ThreadlocalBlockAllocator<T>, TLCACHE_SIZE> cache;

	constexpr static uint32_t buffer_mask = 0x00FF'FFFF;

	uint32_t
	allocate(T&& h)
	{
		auto& allocator = cache.get();
		return allocator.allocate(std::move(h));
	}

	T const&
	get(uint32_t idx) const
	{
		uint32_t buffer_idx = idx >> 24;
		auto const& allocators = cache.get_objects();

		if (!allocators[buffer_idx])
		{
			throw std::runtime_error("invalid HashAllocator access");
		}

		return allocators[buffer_idx] -> get(idx & buffer_mask);
	}

	void
	reset()
	{
		auto& allocs = cache.get_objects();
		for (auto& alloc : allocs)
		{
			if (alloc)
			{
				alloc -> reset();
			}
		}
	}
};

}
