
#pragma once

#include "sandbox_factory.h"

#include <core/sim/scene.h>
#include <core/sim/misc.h>
#include <core_ui/sim/box2d_widget.h>

#include <QKeyEvent>
#include <QMouseEvent>

#include <memory>
#include <unordered_set>
using namespace std;

namespace electric_fence_scene {

class Scene : public sim::Scene {
 public:
  Scene();

  void postStep(float dt) override;
  void onContact(b2Contact* contact) override;
  
 private:
  b2Body* reaper_ = nullptr;
  b2Body* fence_ = nullptr;
  unordered_set<b2Body*> reaped_bodies_;
};

class SceneUi : public physics_ui::Box2dSceneUi {
 public:
  explicit SceneUi(Scene* scene) : scene_(scene) {}

  void mousePressEvent(const QPointF& pos, QMouseEvent* event) override {
    const auto x = float(pos.x());
    const auto y = float(pos.y());
    
    if ((event->buttons() & Qt::LeftButton) != 0) {
      sim::addBall(x, y, 3, scene_->box2dWorld());
    }

    if ((event->buttons() & Qt::RightButton) != 0) {
      sim::addBox(x, y, 10, 2, scene_->box2dWorld());
    }
  }

  void keyPressEvent(QKeyEvent* event) override {
    if (event->key() == Qt::Key_Space) {
      sim::addBullet(-95, 5, 100 * 100, 0, scene_->box2dWorld());
    }
  }

 private:
  Scene* scene_ = nullptr;
};

class Factory : public SandboxFactory {
  SandboxScenePackage createScenePackage(const core::PropertySet*) override {
    auto scene = make_unique<Scene>();
    auto scene_ui = make_unique<SceneUi>(scene.get());
    return { std::move(scene), std::move(scene_ui) };
  }
};

}  // namespace electric_fence_scene
