#include "mempool/mempool.h"

namespace scs {

std::optional<SignedTransaction>
Mempool::get_new_tx()
{
    uint64_t i = indices.fetch_add(1, std::memory_order_acquire);

    uint32_t consumed_idx = i & 0xFFFF'FFFF;
    uint32_t filled_idx = i >> 32;

    if (consumed_idx >= filled_idx) {
        return std::nullopt;
    }
    std::optional<SignedTransaction> out
        = std::move(ringbuffer[consumed_idx % MAX_MEMPOOL_SIZE]);

    return out;
}

uint32_t
Mempool::add_txs(std::vector<SignedTransaction>&& txs)
{
    std::lock_guard lock(mtx);

    uint64_t i = indices.load(std::memory_order_acquire);

    uint32_t consumed_idx = i & 0xFFFF'FFFF;
    uint32_t filled_idx = i >> 32;

    if (consumed_idx > filled_idx)
    {
        consumed_idx = filled_idx;
    }

    // consumed_idx only increases, so this only overestimates space
    uint32_t used_space = filled_idx - consumed_idx;

    // underestimate
    uint32_t write_now
        = std::min<uint32_t>(MAX_MEMPOOL_SIZE - used_space, txs.size());

    for (size_t cpy = 0; cpy < write_now; cpy++) {
        ringbuffer[(filled_idx + cpy) % MAX_MEMPOOL_SIZE] = std::move(txs[cpy]);
    }

    filled_idx = (filled_idx + write_now) % MAX_MEMPOOL_SIZE;
    consumed_idx = consumed_idx % MAX_MEMPOOL_SIZE;

    indices.store((static_cast<uint64_t>(filled_idx) << 32) + consumed_idx,
                      std::memory_order_release);

    return write_now;
}

} // namespace scs
