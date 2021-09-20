// Copyright 2019-2021 Cambridge Quantum Computing
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <catch2/catch.hpp>

#include "CircuitsForTesting.hpp"
#include "Predicates/PassGenerators.hpp"
#include "Simulation/CircuitSimulator.hpp"
#include "Simulation/ComparisonFunctions.hpp"
#include "Transformations/Transform.hpp"

namespace tket {

SCENARIO("Basic >> sequencing") {
  GIVEN("A UCCSD example") {
    auto circ = CircuitsForTesting::get().uccsd;
    REQUIRE(circ.count_gates(OpType::CX) == 12);
    const StateVector s0 = tket_sim::get_statevector(circ);
    (Transform::optimise_via_PhaseGadget() >> Transform::clifford_simp())
        .apply(circ);
    REQUIRE(circ.count_gates(OpType::CX) == 8);
    const StateVector s1 = tket_sim::get_statevector(circ);
    REQUIRE(tket_sim::compare_statevectors_or_unitaries(s0, s1));
  }
}

SCENARIO("List sequencing") {
  GIVEN("A UCCSD example") {
    auto circ = CircuitsForTesting::get().uccsd;
    REQUIRE(circ.count_gates(OpType::CX) == 12);
    REQUIRE(circ.count_gates(OpType::V) == 0);
    REQUIRE(circ.count_gates(OpType::S) == 0);
    REQUIRE(circ.count_gates(OpType::Z) == 0);
    REQUIRE(circ.count_gates(OpType::X) == 0);
    const StateVector s0 = tket_sim::get_statevector(circ);
    std::vector<Transform> seq = {
        Transform::optimise_via_PhaseGadget(), Transform::clifford_simp(),
        Transform::rebase_tket()};
    Transform::sequence(seq).apply(circ);
    REQUIRE(circ.count_gates(OpType::CX) == 8);
    REQUIRE(circ.count_gates(OpType::V) == 0);
    REQUIRE(circ.count_gates(OpType::S) == 0);
    REQUIRE(circ.count_gates(OpType::Z) == 0);
    REQUIRE(circ.count_gates(OpType::X) == 0);
    const StateVector s1 = tket_sim::get_statevector(circ);
    REQUIRE(tket_sim::compare_statevectors_or_unitaries(s0, s1));
  }
}

}  // namespace tket
