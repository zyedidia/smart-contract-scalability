#pragma once

#include "xdr/storage.h"
#include "xdr/types.h"

#include "mtt/trie/merkle_trie.h"

#include <map>
#include <optional>

#include <xdrpp/marshal.h>

#include "object/revertable_object.h"

#include "state_db/new_key_cache.h"

namespace scs {

class DeltaBatch;
class ModifiedKeysList;

class StateDB
{
    static std::vector<uint8_t> serialize(const RevertableObject& v)
    {
        auto const& res = v.get_committed_object();
        if (res) {
            return xdr::xdr_to_opaque(*res);
        }
        return {};
    }

    using base_value_struct = trie::PointerValue<RevertableObject, &serialize>;

    struct value_struct : public base_value_struct
    {
        value_struct(const StorageObject& obj)
            : base_value_struct(std::make_unique<RevertableObject>(obj))
        {}

        value_struct()
            : base_value_struct()
        {}
    };

  public:
    using prefix_t = trie::ByteArrayPrefix<sizeof(AddressAndKey)>;
    using metadata_t
        = trie::CombinedMetadata<trie::SizeMixin, trie::DeletableMixin>;
    using value_t
        = value_struct;
    using trie_t = trie::MerkleTrie<prefix_t, value_t, metadata_t>;
    using cache_t = utils::ThreadlocalCache<trie_t>;

  private:
    trie_t state_db;
    NewKeyCache new_key_cache;

    std::atomic<bool> has_uncommitted_deltas = false;

    void assert_not_uncommitted_deltas() const;

  public:
    std::optional<StorageObject> get_committed_value(
        const AddressAndKey& a) const;

    std::optional<RevertableObject::DeltaRewind> try_apply_delta(
        const AddressAndKey& a,
        const StorageDelta& delta);

    void commit_modifications(const ModifiedKeysList& list);

    void rewind_modifications(const ModifiedKeysList& list);

    Hash hash();

#if 0
// old
	void 
	populate_delta_batch(DeltaBatch& delta_batch) const;

	void 
	apply_delta_batch(DeltaBatch const& delta_batch);
#endif
   
};

} // namespace scs
