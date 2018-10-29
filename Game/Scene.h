//
// Scene.h
//

#pragma once

#include "SceneNode.h"
#include "StepTimer.h"


class Scene
{
    protected:
        Scene() = default;

    public:
        virtual void CreateDeviceDependentResources() {}
        virtual void CreateWindowSizeDependentResources() {}

        // IDeviceNotify
        virtual void OnDeviceLost() {}
        virtual void OnDeviceRestored() {}

        // Messages
        virtual void OnActivated() {}
        virtual void OnDeactivated() {}
        virtual void OnSuspending() {}
        virtual void OnResuming() {}
        virtual void OnWindowMoved() {}
        virtual void OnWindowSizeChanged(int width, int height) {}

        virtual bool Load(ID3D11DeviceContext1* deviceContext) = 0;
        virtual void Unload() = 0;

        virtual void Update(DX::StepTimer const& timer) = 0;
        virtual void Render() = 0;

        virtual void Pause() = 0;
        virtual void Resume() = 0;

    protected:
        // All scenes must have Scene Graph.
        // Scene Graph is just a root node of every scene.
        SceneNode* sceneGraph;

        // @todo: make it possible to switch scenes from any scene.
        // TIP: SceneManager* sceneManager; // something like that.

};