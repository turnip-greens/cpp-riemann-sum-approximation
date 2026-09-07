#include <iostream>
#include <cstdint>
#include <string>
#include <cmath>
#include <format>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

static float s3 = 1.f, s2 = 0.f, s1 = 0.f, c = 0.f;
static double lower_bound = 0.f, upper_bound = 1.f;
static int samples = 8192;
static bool successfully_calculated = false;
static double answer = 0.f;

struct monomial
{
	float scalar;
	float exponent;

	double calc(float x)
	{
		return scalar * powf(x, exponent);
	}

	monomial(float s, float e) : scalar(s), exponent(e) {}
};

struct standard_form_3
{
	monomial m[3];
	float constant;

	double calc(float x)
	{
		return m[0].calc(x) + m[1].calc(x) + m[2].calc(x) + constant;
	}
	
	standard_form_3(float s3, float s2, float s1, float c)
		: m{ {s3, 3}, {s2, 2}, {s1, 1} }, constant(c) {}
};

std::string format(std::string s, float v)
{
	if (v == 0.f)
		return "";
	else if (v == 1.f)
		if (s == "")
			return " + 1";
		else
			return " + " + s;
	else if (v == -1.f)
		if (s == "")
			return " - 1";
		else
			return " - " + s;
	else if (v < 0.f)
		return std::format(" - {}{}", std::abs(v), s);
	else
		return std::format(" + {}{}", v, s);
}

void render_frame(void* window_ptr)
{
    GLFWwindow* window = static_cast<GLFWwindow*>(window_ptr);

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration | 
                            ImGuiWindowFlags_NoMove | 
                            ImGuiWindowFlags_NoResize | 
                            ImGuiWindowFlags_NoBringToFrontOnFocus;

    ImGui::Begin("MainContainer", nullptr, flags);
    ImGui::PopStyleVar(2);

    ImGui::Text("Define Function: ");
    ImGui::Text("f(x) = ax³ + bx² + cx + d");
    
    ImGui::Text("a: "); ImGui::SameLine();
    ImGui::InputFloat("##a", &s3, 0.0f, 0.0f, "%.1f");
    
    ImGui::Text("b: "); ImGui::SameLine();
    ImGui::InputFloat("##b", &s2, 0.0f, 0.0f, "%.1f");
    
    ImGui::Text("c: "); ImGui::SameLine();
    ImGui::InputFloat("##c", &s1, 0.0f, 0.0f, "%.1f");
    
    ImGui::Text("d: "); ImGui::SameLine();
    ImGui::InputFloat("##d", &c, 0.0f, 0.0f, "%.1f");
    
    std::string func_str = "f(x) =" + 
                           format("x³", s3) + 
                           format("x²", s2) + 
                           format("x", s1) + 
                           format("", c);

    ImGui::TextUnformatted(func_str.c_str());
    ImGui::Separator();

    ImGui::Text("Define Interval: ");
    ImGui::Text("Lower Bound: "); ImGui::SameLine();
    ImGui::InputDouble("##lower_bound", &lower_bound, 0.0f, 0.0f, "%.1f");
    
    ImGui::Text("Upper Bound: "); ImGui::SameLine();
    ImGui::InputDouble("##upper_bound", &upper_bound, 0.0f, 0.0f, "%.1f");
    
    ImGui::Text("Samples: "); ImGui::SameLine();
    ImGui::InputInt("##samples", &samples, 0, 0);

    if (ImGui::Button("Calculate"))
    {
        double left_sum = 0.0;
        double right_sum = 0.0;
        double step_size = (upper_bound - lower_bound) / samples;
        
        standard_form_3 function(s3, s2, s1, c);

        for (int i = 0; i < samples; i++)
        {
            left_sum += function.calc(lower_bound + i * step_size) * step_size;
            right_sum += function.calc(lower_bound + (i + 1) * step_size) * step_size;
        }

        answer = (left_sum + right_sum) / 2.0;
        successfully_calculated = true;
    }

    if (successfully_calculated)
    {
        ImGui::Text("Area under curve: %.6f", answer);
    }

    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW\n";
		return -1;
	}
		
#ifdef __EMSCRIPTEN__
    // WebGL 2.0 configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#else
    // Desktop OpenGL configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif



	GLFWwindow* window = glfwCreateWindow(800, 600, "Integral Calculator", nullptr, nullptr);
	if (!window) 
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

#ifndef __EMSCRIPTEN__
    static const ImWchar math_ranges[] = {
        0x0020, 0x00FF,
        0x2070, 0x209F,
        0x2200, 0x22FF,
        0,
    };
    ImFont* font = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.0f, nullptr, math_ranges);
    if (font == nullptr) 
    {
        io.Fonts->AddFontDefault();
    }
#else
    // WebAssembly uses standard default font unless preloaded via virtual filesystem
    io.Fonts->AddFontDefault();
#endif
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);

#ifdef __EMSCRIPTEN__
    ImGui_ImplOpenGL3_Init("#version 300 es");
    // Hand execution loop control to the browser's requestAnimationFrame
    emscripten_set_main_loop_arg(render_frame, window, 0, true);
#else
    ImGui_ImplOpenGL3_Init("#version 330");
    while (!glfwWindowShouldClose(window))
    {
        render_frame(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
#endif

	return 0;
}
