#include "chipset/Clock.h"
#include "cpu/CPU.h"
#include "memory/Memory.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <cstdint>
#include <print>



auto main() -> std::uint32_t
{
	std::println("NutVIC Emulator");

	if (glfwInit() == GLFW_FALSE)
	{
		std::println("Failed to init glfw");
		return -1;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "NutVIC", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	gladLoadGL();

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	// Init system
	nvic::Clock clock{nvic::Clock::FrequencyMode::NTSC};
	nvic::CPU cpu{clock};
	nvic::Memory memory{ cpu.GetSignals() };

	nvic::MemoryChunk charrom{ .ReadOnly{true} };
	memory.SetChunk(0, charrom);

	clock.Start();
	cpu.Start();


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplGlfw_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();


		{
			ImGui::Begin("Control");

			if (ImGui::Button("Step"))
			{
				cpu.SetRunMode(nvic::CPU::RunMode::Step);
				cpu.SingleShot();
			}

			if (ImGui::Button("Run"))
			{
				cpu.SetRunMode(nvic::CPU::RunMode::Continuous);
			}

			ImGui::End();
		}




		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	cpu.Stop();
	clock.Stop();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
