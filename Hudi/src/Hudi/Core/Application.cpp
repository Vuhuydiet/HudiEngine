#include "hdpch.h"
#include "Application.h"

#include "Hudi/Core/Input.h"

#include <ECS.h>

#include "Platform/SDL2/imgui_impl_sdl2.h"
#include "Platform/SDL2/imgui_impl_sdlrenderer.h"

namespace Hudi {

	Application::Application()
	{
		// Init Log
		Hudi::Log::Init();

		// Init SDL
		SDL_Init(SDL_INIT_EVERYTHING);

		// Init ECS
		ECS::Init_Everything();

	}

	void Application::Init()
	{
		// Create Window
		m_Window = Ref<Window>(Window::Create(WindowProperties()));
        Renderer::Init(m_Window->GetSDL_Window());

		m_PhysicsSystem = ECS::Coordinator::RegisterSystem<PhysicsSystem>();

		m_RenderSystem = ECS::Coordinator::RegisterSystem<RenderSystem>();
		m_RenderSystem->Init(m_Window->GetWindowProps());

		// Start the app
		m_Running = true;
	}

	Application::~Application()
	{
        ShutDown();
	}

    void Application::ShutDown()
    {
        Renderer::ShutDown();

        // Destroy ECS
        ECS::Destroy_Everything();
        HD_CORE_INFO("Destroyed ECS in Application.cpp!");
    }

	WindowProps Application::WindowProperties()
	{
		return WindowProps();
	}


	void Application::OnAwake()
	{
		// Awake Behaviours
		ECS::Coordinator::OnAwakeComponents();
	}

	void Application::OnEvent()
	{
		m_Window->OnUpdate();

		// ------------------------- //
		if (Input::Quit())
			CloseApplication();
		
		for (Event event; m_Window->PollEvent(event); )
		{
			for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
			{
				if (event.handled)
					break;
				(*it)->OnEvent(event);
			}
		}
	}

	void Application::OnUpdate()
	{
		// Update Behaviours
		ECS::Coordinator::OnUpdateComponents();

		// Update Systems
		ECS::Coordinator::OnUpdateSystem<PhysicsSystem>();
		ECS::Coordinator::OnUpdateSystem<RenderSystem>();
	}

	void Application::OnRender()
	{
		m_RenderSystem->OnRender();
	}

	void Application::Run()
	{
        //ImGui::CreateContext();
        //ImGuiIO& io = ImGui::GetIO(); (void)io;
        ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        ////io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        //// Setup Dear ImGui style
        //ImGui::StyleColorsDark();
        ////ImGui::StyleColorsLight();

        //// Setup Platform/Renderer backends
        //ImGui_ImplSDL2_InitForSDLRenderer(m_Window->GetSDL_Window(), Renderer::GetSDL_Renderer());
        //ImGui_ImplSDLRenderer_Init(Renderer::GetSDL_Renderer());

        //// Load Fonts
        //// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        //// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        //// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        //// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        //// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        //// - Read 'docs/FONTS.md' for more instructions and details.
        //// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        ////io.Fonts->AddFontDefault();
        ////io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        ////io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        ////io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        ////io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        ////ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
        ////IM_ASSERT(font != NULL);

        //// Our state
        //bool show_demo_window = true;
        //bool show_another_window = false;
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        //// Main loop
        //bool done = false;
        //while (!done)
        //{
        //    Timer timer(Time::deltaTime());
        //    // Poll and handle events (inputs, window resize, etc.)
        //    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        //    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        //    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        //    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        //    SDL_Event event;
        //    while (SDL_PollEvent(&event))
        //    {
        //        ImGui_ImplSDL2_ProcessEvent(&event);
        //        if (event.type == SDL_QUIT)
        //        {
        //            done = true;
        //        }
        //        /*if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(m_Window->GetSDL_Window()))
        //            done = true;*/
        //    }

        //    // Start the Dear ImGui frame
        //    ImGui_ImplSDLRenderer_NewFrame();
        //    ImGui_ImplSDL2_NewFrame();
        //    ImGui::NewFrame();

        //    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        //    if (show_demo_window)
        //        ImGui::ShowDemoWindow(&show_demo_window);

        //    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        //    {
        //        static float f = 0.0f;
        //        static int counter = 0;

        //        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        //        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        //        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        //        ImGui::Checkbox("Another Window", &show_another_window);

        //        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        //        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        //        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        //            counter++;
        //        ImGui::SameLine();
        //        ImGui::Text("counter = %d", counter);

        //        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        //        ImGui::End();
        //    }

        //    // 3. Show another simple window.
        //    if (show_another_window)
        //    {
        //        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        //        ImGui::Text("Hello from another window!");
        //        if (ImGui::Button("Close Me"))
        //            show_another_window = false;
        //        ImGui::End();
        //    }

        //    // Rendering
        //    ImGui::Render();
        //    SDL_RenderSetScale(Renderer::GetSDL_Renderer(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        //    SDL_SetRenderDrawColor(Renderer::GetSDL_Renderer(), (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
        //    SDL_RenderClear(Renderer::GetSDL_Renderer());
        //    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        //    SDL_RenderPresent(Renderer::GetSDL_Renderer());

            Preprocess();

            SceneManager::GetActiveScene().BeginScene();
            while (IsRunning())
            {
                Timer timer(Time::deltaTime());

                OnAwake();
                OnEvent();
                OnUpdate();
                OnRender();

                ECS::Coordinator::OnUpdate();
            }
            SceneManager::GetActiveScene().EndScene();
        //}
        /*ImGui_ImplSDLRenderer_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();*/
	}

}