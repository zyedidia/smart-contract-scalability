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

#include <catch2/catch_test_macros.hpp>

#include "crypto/lthash.h"

#include "xdr/types.h"

using namespace scs;

TEST_CASE("lthash basic", "[lthash]")
{
    uint64_t base_val = 0;

    LtHash16 h(base_val);
}

TEST_CASE("lthash different", "[lthash]")
{
    uint64_t b1 = 0, b2 = 1, b3 = 0;

    LtHash16 h1(b1), h2(b2), h3(b3);

    REQUIRE(b1 == b3);
    REQUIRE(b1 != b2);
}

TEST_CASE("lthash arithmetic", "[lthash]")
{
    uint64_t b1 = 0, b2 = 1;

    LtHash16 h1(b1), h2(b2), g1, g2;

    REQUIRE(g1 != h1);

    g1 += h1;

    REQUIRE(g1 == h1);

    g2 += h2;

    REQUIRE(g2 == h2);

    REQUIRE(g1 != g2);

    g1 += h2;
    g2 += h1;

    REQUIRE(g1 == g2);
}

TEST_CASE("compare avx", "[lthash]")
{
    uint64_t b1 = 0, b2 = 1;

    using LtHash16Basic = LtHash<1024, 16, detail::ArithmeticEngine::Basic>;

    LtHash16 h1(b1), h2(b2), g1, g2;
    LtHash16Basic hb1(b1), hb2(b2), gb1, gb2;

    REQUIRE(h1.get_hash() == hb1.get_hash());
    REQUIRE(h2.get_hash() == hb2.get_hash());

    g1 += h1;
    g2 += h2;

    gb1 += hb1;
    gb2 += hb2;

    REQUIRE(g1.get_hash() == gb1.get_hash());
    REQUIRE(g2.get_hash() == gb2.get_hash());

    gb1 += hb2;
    gb2 += hb1;

    g1 += h2;
    g2 += h1;

    REQUIRE(g1.get_hash() == gb2.get_hash());
    REQUIRE(g2.get_hash() == gb1.get_hash());
}
