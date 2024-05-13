#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <iostream>
#include <string>
//#include <Windows.h>
#include <thread>
#include "Pipeline.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 1024;
int fps = 0;


//pram
Pipeline pip(SCR_WIDTH, SCR_HEIGHT,RenderMode::MESH);
Camera camera(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 0.1f, 100.0f);
glm::vec4 refreshColor(30, 80, 90, 255);
glm::vec4 Black(0, 0, 0, 255);


// void ShowFps(GLFWwindow* window) {
//     while (1) {
//         Sleep(1000);
//         std::string text = "MyRender fps:" + std::to_string(fps);
//         glfwSetWindowTitle(window, text.c_str());
//         fps = 0;
//     }
// }


int main(int argc, char** argv) {
    if (!glfwInit())   
        return 1;

    const char* glsl_version = "#version 330";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 1600, "Kci_Renderer", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);    

    
    //pipeline
    pip.init();
    pip.setShaderMode(Simple);
    pip.setCamera(&camera);

    // lighting
    Material material;
    material.setMaterial(glm::vec3(0.1, 0.1, 0.1),
        glm::vec3(0.5, 0.5, 0.5),
        glm::vec3(0.8, 0.8, 0.8),
        16.0);
    pip.setMaterial(&material);
    //directionlight
    //pip.setDirectionLight(
    //    glm::vec3(1,1,1),
    //    glm::vec3(0.9,0.1,0.1),
    //    glm::vec3(0.9,0.1,0.1),
    //    glm::vec3(-1,-2,-1));
    //pointlight
    //pip.setPointLight(
    //    glm::vec3(2, 2, 2),
    //    glm::vec3(0.9, 0.1, 0.1),
    //    glm::vec3(0.9, 0.1, 0.1),
    //    glm::vec3(0.0, 0.0, 0.0),
    //    glm::vec3(1.0f, 0.07f, 0.017f));

    //model
    //Model model("./image/floor/floor.obj");
    //pip.loadTexture("./image/floor/floor_diffuse.tga");
	Model model("D:\\Kci_Renderer\\SoftRenderer\\image\\african_head\\african_head.obj");
    pip.loadTexture("D:\\Kci_Renderer\\SoftRenderer\\image\\african_head\\african_head_diffuse.tga");

    pip.bindTexture(0);
    pip.add(&model);

    // std::thread t(ShowFps, window);
    // t.detach();

    float angle = 0.0f;


    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glfwPollEvents();

        pip.fillColorBuffer(Black);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        pip.setModelMatrix(glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)));
    	pip.drawObject(&model);
        pip.display();
        pip.swapBuffer();
        angle += 10.0f;
        // fps++;

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        camera.rotatePitch(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        camera.rotateYaw(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        camera.rotateYaw(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        camera.rotatePitch(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveForward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveBackward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveLeft(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveRight(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera.moveUp(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera.moveDown(0.1);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    pip.resize(width, height);
    glViewport(0, 0, width, height);
}