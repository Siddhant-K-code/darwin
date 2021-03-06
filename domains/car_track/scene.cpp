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

#include "scene.h"

#include <core/math_2d.h>

#include <math.h>

namespace car_track {

static b2Vec2 toBox2dVec(const math::Vector2d& v) {
  return b2Vec2(float(v.x), float(v.y));
}

Scene::Scene(const sim::Track* track, const CarTrack* domain)
    : sim::Scene(b2Vec2(0, 0), sim::Rect(-kWidth / 2, -kHeight / 2, kWidth, kHeight)),
      track_(track),
      domain_(domain) {
  const auto& config = domain_->config();

  // instantiate the track
  track_->createFixtures(&world_);

  // calculate the start position
  const auto start_node = track_->distanceToNode(0);
  const auto start_pos = start_node.offset(-config.track_width / 2);
  const auto start_angle = atan2(start_node.n.y, start_node.n.x);

  // create the car
  sim::CarConfig car_config = domain_->carConfig();
  car_config.position = toBox2dVec(start_pos);
  car_config.angle = float(start_angle);
  car_ = make_unique<sim::Car>(&world_, car_config);

  // attach a light to the car's body
  b2LightDef light_def;
  light_def.body = car_->body();
  light_def.color = b2Color(1, 1, 1);
  light_def.intensity = 2.0f;
  light_def.attenuation_distance = 10.0f;
  world_.CreateLight(&light_def);
}

float Scene::fitness() const {
  return float(distance_) / track_->nodesCount();
}

void Scene::preStep() {
  car_->preStep();
}

void Scene::postStep(float dt) {
  car_->postStep(dt);
  distance_ = track_->updateTrackDistance(distance_, car_->body()->GetPosition());
  updateVariables();
}

void Scene::updateVariables() {
  const b2Body* car_body = car_->body();
  variables_.car_x = car_body->GetPosition().x;
  variables_.car_y = car_body->GetPosition().y;
  variables_.car_vx = car_->actualVelocity().x;
  variables_.car_vy = car_->actualVelocity().y;
  variables_.car_dir = car_body->GetAngle();
  variables_.distance = distance_;
}

}  // namespace car_track
