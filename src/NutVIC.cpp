#include "chipset/CIA.h"
#include "chipset/Clock.h"
#include "chipset/PLA.h"
#include "chipset/VIC_II.h"
#include "cpu/CPU.h"
#include "cpu/Signals.h"
#include "display/Texture.h"
#include "memory/Memory.h"
#include "ui/CPUPanel.h"
#include "ui/MemoryPanel.h"
#include "ui/VICPanel.h"
#include "utils/FileTools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <array>
#include <cstdint>
#include <print>
#include <vector>



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
	nvic::Signals signals{};
	nvic::CPU cpu{ signals };
	nvic::PLA pla{ signals };
	nvic::Memory memory{ pla, signals, cpu.GetRegisters() };
	cpu.SetMemory(&memory);

	std::array<nvic::MemoryChunk, 16> ram{};

	for (auto i = 0; i < 16; i++)
		memory.SetChunk(i, 0, ram.at(i));

	// Set IO #CS = active high
//	memory.SetChunk(0xD, 1, ram.at(0xD));

	std::vector<nvic::MemoryChunk> basicROM{};
	std::vector<nvic::MemoryChunk> kernalROM{};
	std::vector<nvic::MemoryChunk> chargenROM{};

	nvic::MemoryChunk ioData{ .ReadWrite{true} };

	memory.SetChunk(0xD, 2, ioData);

	nvic::LoadROM(basicROM, "rom/c64/basic", 0xA000);
	nvic::LoadROM(chargenROM, "rom/c64/chargen", 0xD000);
	nvic::LoadROM(kernalROM, "rom/c64/kernal", 0xE000);

	memory.SetChunk(0xA, 1, basicROM);
	memory.SetChunk(0xD, 1, chargenROM);
	memory.SetChunk(0xE, 1, kernalROM);

	memory.SetDefaultValues();
	cpu.Reset();

	nvic::CIA cia1{ 0, ioData, signals, cpu.GetRegisters() };
	nvic::CIA cia2{ 1, ioData, signals, cpu.GetRegisters() };

	nvic::VIC_II vic2{ ioData, chargenROM[0], ram, signals};

	nvic::Clock clock{ nvic::Clock::FrequencyMode::NTSC, signals, cpu, vic2, cia1, cia2 };

	nvic::CPUPanel cpuPanel{ cpu.GetRegisters(), signals };
	nvic::MemoryPanel memoryPanel{ memory, cpu.GetRegisters() };
	nvic::VICPanel vicPanel{ vic2 };

//	nvic::Texture displayTexture{ 320, 200 };
	nvic::Texture displayTexture{ 418, 251 };

//	nvic::MemoryChunk charrom{ .ReadWrite{true} };
//	memory.SetChunk(0, charrom);

	clock.Start();
//	cpu.Start();
//	vic2.Start();

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

			ImGui::Text("Host CPU Frequency: %llu", clock.GetHostFrequency());

			ImGui::Separator();

			if (ImGui::Button("Step"))
			{
				cpu.SetRunMode(nvic::CPU::RunMode::Step);
				cpu.SingleShot();
			}

			if (ImGui::Button("Run"))
			{
				cpu.SetRunMode(nvic::CPU::RunMode::Continuous);
			}

			if (ImGui::Button("Jump to"))
			{
				signals.JumpTo = true;
			}

			ImGui::SameLine();

			ImGui::InputInt("Address", &signals.JumpToAddress, 1, 0x100, ImGuiInputTextFlags_CharsHexadecimal);


			if (ImGui::Button("Reset"))
			{
				cpu.Reset();
			}

			if (ImGui::Button("Interrupt request"))
			{
				signals.InterruptRequest = true;
			}

			if (cpu.GetRunMode() == nvic::CPU::RunMode::Step)
			{
				ImGui::Separator();
//				ImGui::Text("Cycles: %d", cpu.GetLastExecutionCycles());
//				ImGui::Text("Execution time: %llu", cpu.GetLastExecutionTime());
//				ImGui::Text("Decoder time in cycles: %llu", cpu.GetDecoderCycleTime());
				ImGui::Text("Host execution time in cycles: %llu", cpu.GetDecoderHostExecutionTime());				
				ImGui::Text("Host cycles for opcode: %d", cpu.GetLastExecutionCycles() * clock.CPUClockDelay());
				ImGui::Text("Decoder time in host cycles: %llu", cpu.GetDecoderHostCycleTime());
				ImGui::Text("Host cycles per CPU cycle: %lu", clock.CPUClockDelay());

				ImGui::Text("Ops run: %llu", cpu.GetOpsRun());
			}
			else
			{
				ImGui::Separator();
				ImGui::Text("Ticks per second: %llu", clock.GetTicksPerSecond());
				ImGui::Text("Utilization: %f %%", clock.GetUtilization());
			}

			ImGui::End();
		}

		{
			ImGui::Begin("Screen");

			displayTexture.SetData(vic2.GetImageData());
//			ImGui::Image(displayTexture.GetTexture(), ImVec2{ 640, 400 });
			ImGui::Image(displayTexture.GetTexture(), ImVec2{ 418, 235 });

			ImGui::End();
		}
		
		cpuPanel.Draw();
		memoryPanel.Draw();
		vicPanel.Draw();

		vic2.Update();

		cia1.Update();
		cia2.Update();
//		pla.Update();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

//	vic2.Stop();
//	cpu.Stop();
	clock.Stop();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
