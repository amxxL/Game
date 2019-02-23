//
// Application.cpp
//

#include "pch.h"
#include "Application.h"
#include "Color.h"

//using Microsoft::WRL::ComPtr;

Application::Application() noexcept(false)
{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Application::Initialize(int width, int height)
{
    m_window = std::make_unique<Window>(L"Game", width, height);
    m_window->RegisterUserData(reinterpret_cast<LONG_PTR>(this));

    m_deviceResources->SetWindow(m_window->GetHandle(), width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Scenes manager.
    m_sceneManager = std::make_unique<SceneManager>(m_deviceResources->GetD3DDeviceContext());
    m_sceneManager->PushScene(PlayScene::Instance());

    // TIP: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    m_keyboard = std::make_unique<DirectX::Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    //m_mouse->SetWindow(window);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Init(m_window->GetHandle());
    ImGui_ImplDX11_Init(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());
    ImGui::StyleColorsDark();

}

#pragma region Frame Update
// Executes the basic game loop.
void Application::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Application::Update(DX::StepTimer const& timer)
{
    // TODO: Add your game logic here.
    auto kb = m_keyboard->GetState();
    if (kb.Escape)
        PostQuitMessage(0);

    //auto mouse = m_mouse->GetState();

    /*
    while (!m_mouse->EventBufferIsEmpty())
    {
        Mouse::Event mouseEvent = m_mouse->ReadEvent();

        if (mouseEvent.GetType() == Mouse::Event::MoveRaw)
        {
            std::wostringstream ss(L"");
            ss << "X: " << mouseEvent.GetPositionX() << " Y: "
                << mouseEvent.GetPositionY();
            m_window->SetTitle(ss.str().c_str());
        }
    }*/

    m_sceneManager->Update(timer);
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Application::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
        return;

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    m_sceneManager->Render();

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Application::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    context->ClearRenderTargetView(renderTarget, color);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Application::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Application::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Application::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Application::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Application::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Application::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.

}

// Properties
void Application::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Application::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Initialize device dependent objects here (independent of window size).
    device;
    context;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Application::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
}

void Application::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Application::OnDeviceRestored()
{
    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}
#pragma endregion