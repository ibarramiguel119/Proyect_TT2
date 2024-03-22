// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015-2017 Intel Corporation. All Rights Reserved.

#include <librealsense2/rs.hpp> // Include RealSense Cross Platform API
#include "example.hpp"          // Include short list of convenience functions for rendering

#include "example-imgui.hpp"




#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstring>


#define IM_ARRAYSIZE(_ARR) ((int)(sizeof(_ARR)/sizeof(*_ARR)))
         // std::min, std::max

// Helper functions
void render_slider(rect location);
void register_glfw_callbacks(window& app, glfw_state& app_state);

int main(int argc, char * argv[]) try
{
    // Create a simple OpenGL window for rendering:
    window app(1280, 720, "R3Dsystem");
    ImGui_ImplGlfw_Init(app, false);// ImGui library intializition


    // Construct an object to manage view state
    glfw_state app_state;
    // register callbacks to allow manipulation of the pointcloud
    register_glfw_callbacks(app, app_state);

    // Declare pointcloud object, for calculating pointclouds and texture mappings
    rs2::pointcloud pc;
    // We want the points object to be persistent so we can display the last cloud when a frame drops
    rs2::points points;

    // Declare RealSense pipeline, encapsulating the actual device and sensors
    rs2::pipeline pipe;
    // Start streaming with default recommended configuration
    pipe.start();

    while (app) // Application still alive?
    {
        // Wait for the next set of frames from the camera
        auto frames = pipe.wait_for_frames();

        auto color = frames.get_color_frame();

        // For cameras that don't have RGB sensor, we'll map the pointcloud to infrared instead of color
        if (!color)
            color = frames.get_infrared_frame();

        // Tell pointcloud object to map to this color frame
        pc.map_to(color);

        auto depth = frames.get_depth_frame();

        // Generate the pointcloud and texture mappings
        points = pc.calculate(depth);

        // Upload the color frame to OpenGL
        app_state.tex.upload(color);

        // Draw the pointcloud
        draw_pointcloud(app.width(), app.height(), app_state, points);

	    float w = static_cast<float>(app.width());
        float h = static_cast<float>(app.height());

	    ImGui_ImplGlfw_NewFrame(1);
        render_slider({ 5.f, 0, w, h }); 
        ImGui::Render();


        //Decalaraciones del variables de prueba 

        char buf[256];
        float f = 0.0f;

        ImGui::Text("Hello, world %d", 123);
        if (ImGui::Button("Save"))
            std::cout << "Hola inge" << std::endl;
        ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        
    }

    return EXIT_SUCCESS;
}
catch (const rs2::error & e)
{
    std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
    return EXIT_FAILURE;
}
catch (const std::exception & e)
{
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
}



void render_slider(rect location)
{
    // Some trickery to display the control nicely
    static const int flags = ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove;
    const int pixels_to_buttom_of_stream_text = 25;
    const float slider_window_width = 30;

    ImGui::SetNextWindowPos({ location.x, location.y + pixels_to_buttom_of_stream_text });
    ImGui::SetNextWindowSize({ slider_window_width + 20, location.h - (pixels_to_buttom_of_stream_text * 2) });

    //Render the vertical slider
    ImGui::Begin("slider", nullptr, flags);
    ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(215.f / 255, 215.0f / 255, 215.0f / 255));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImColor(215.f / 255, 215.0f / 255, 215.0f / 255));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImColor(215.f / 255, 215.0f / 255, 215.0f / 255));
    auto slider_size = ImVec2(slider_window_width / 2, location.h - (pixels_to_buttom_of_stream_text * 2) - 20);
    ImGui::PopStyleColor(3);

    //Display bars next to slider
    float bars_dist = (slider_size.y / 6.0f);
    for (int i = 0; i <= 6; i++)
    {
        ImGui::SetCursorPos({ slider_size.x, i * bars_dist });
        std::string bar_text = "- " + std::to_string(6-i) + "m";
        ImGui::Text("%s", bar_text.c_str());
    }
    ImGui::End();
}
