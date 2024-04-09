#pragma once

class Scene {
private:
    Scene() = delete;
public:
    virtual bool init();
    virtual void update();
};