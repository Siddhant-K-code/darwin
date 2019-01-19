// Copyright 2019 The Darwin Neuroevolution Framework Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "cgp.h"
#include "genotype.h"
#include "brain.h"

namespace cgp {

Genotype::Genotype() {
  reset();
}

void Genotype::reset() {
  darwin::Genotype::reset();
  // TODO
}

unique_ptr<darwin::Brain> Genotype::grow() const {
  return make_unique<Brain>(this);
}

unique_ptr<darwin::Genotype> Genotype::clone() const {
  return make_unique<Genotype>(*this);
}

json Genotype::save() const {
  json json_obj;
  // TODO
  return json_obj;
}

void Genotype::load(const json& json_obj) {
  Genotype tmp_genotype;
  // TODO
  std::swap(*this, tmp_genotype);
}

}  // namespace cgp
