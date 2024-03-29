// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2021 Intel Corporation. All Rights Reserved.

#pragma once

#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>
#include "example.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>




//Prototipo FUnciones 

//Funciones prototipo 
std::vector<double> grados_a_radianes(const std::vector<int>& grados);
void CalcularGrados(int Altitud, int Asimuth, int Roll ,int& GAltitude, int& GAsimut, int& GRoll );
std::vector<int> CalcularArreglo1(int GAltitude);
std::vector<int> CalcularArreglo2(int GAsimut);
std::vector<int> CalcularArreglo3(int GRoll);
std::vector<std::vector<int>>CalcularPuntosMovimiento(const std::vector<int>& array1, const std::vector<int>& array2, int radio);
std::vector<std::vector<int>> CalcularPuntosCinematicaInversa(const std::vector<std::vector<int>>& result, int si);
std::tuple<double, double, double> sph2cart(double azimuth, double elevation, double radius);


//////////////////////////////
// ImGui Helpers            //
//////////////////////////////

class selection_images{
public:
    selection_images(const char* name, ImVec2 position, ImVec2 size) :
        _name(name), _position(position), _size(size) {}

    void show() {
        ImGui::SetNextWindowPos(_position);
        ImGui::SetNextWindowSize(_size);

        if (ImGui::Begin(_name, nullptr,ImGuiWindowFlags_NoResize)) {
            // Aquí puedes agregar cualquier elemento de la barra de herramientas usando ImGui

            if (ImGui::Button("20")) {
                // Lógica para el botón 1
            }
            ImGui::SameLine();

            if (ImGui::Button("40")) {
                // Lógica para el botón 2
            }
            ImGui::SameLine();
            if (ImGui::Button("60")) {
                // Lógica para el botón 2
            }
           
            if (ImGui::Button("100")) {
                // Lógica para el botón 2
            }
            ImGui::SameLine();
            if (ImGui::Button("120")) {
                // Lógica para el botón 2
            }
            ImGui::SameLine();
            if (ImGui::Button("120")) {
                // Lógica para el botón 2
            }


            // Puedes añadir más elementos según sea necesario

        }
        ImGui::End();
    }

private:
    const char* _name;
    ImVec2 _position;
    ImVec2 _size;
};






//slider for ImGui
//Lista de clases para para los componentes 
class slider {
public:
    slider(const char* name, int seq_id, float init_value, float min_value, float max_value, ImVec2 position, ImVec2 size) :
        _name(name), _seq_id(seq_id), _value(init_value), _min_value(min_value), _max_value(max_value), _position(position), _size(size) {}

    void virtual show()=0;

public:
    const char* _name;
    int _seq_id;
    float _value;
    float _max_value;
    float _min_value;
    ImVec2 _position;
    ImVec2 _size;

};

class imgui_slider : public slider {
public:
    imgui_slider(const char* name, int seq_id, float init_value, float min_value, float max_value, ImVec2 position, ImVec2 size) :
        slider(name, seq_id, init_value, min_value, max_value, position, size) {}

    void show() override {
        ImGui::SetNextWindowSize(_size);
        ImGui::SetNextWindowPos(_position);
        ImGui::Begin(_name, nullptr, _sliders_flags_1);
        if (ImGui::SliderFloat(_name, &_value, _min_value, _max_value)) {
            std::cout << "Slider value: " << _value << std::endl;
        }
    }

    float getValue() const {
        return _value;
    }

public:
    const static int _sliders_flags_1= ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBringToFrontOnFocus;     
};










class imgui_text_input : public slider {
public:
    imgui_text_input(const char* name, int seq_id, const char* init_value, ImVec2 position, ImVec2 size) :
        slider(name, seq_id, 0.0f, 0.0f, 0.0f, position, size) {
            strncpy(_text, init_value, sizeof(_text));
        }
    void show() override {
        ImGui::SetNextWindowSize(_size);
        ImGui::SetNextWindowPos(_position);
        ImGui::Begin(_name, nullptr, _text_input_flags);
        ImGui::InputText("##text_input", _text, sizeof(_text));
    }

    const char* getText() const {
        return _text;
    }

public:
    const static int _text_input_flags = ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBringToFrontOnFocus;   
    char _text[256]; // Adjust size as needed
};

class hdr_slider : public slider {
public:
    hdr_slider(const char* name, int seq_id, float init_value, rs2::sensor& sensor,
        rs2_option option, rs2::option_range range, ImVec2 position, ImVec2 size) : slider(name, seq_id, init_value, range.min, range.max, position, size),
        _sensor(sensor), _option(option), _range(range){}

    void show() override 
    {
        ImGui::SetNextWindowSize(_size);
        ImGui::SetNextWindowPos(_position);
        //concate the name given with seq_id in order to make a unique name (uniqeness is needed for Begin())
        std::string name_id = std::string(_name) + std::to_string(_seq_id);
        ImGui::Begin(name_id.c_str(), nullptr, _sliders_flags);
        ImGui::Text("%s",_name);
        bool is_changed =
            ImGui::SliderFloat("", &_value, _min_value, _max_value, "%.3f", 5.0f, false); //5.0f for logarithmic scale 
        if (is_changed) {
            _sensor.set_option(RS2_OPTION_SEQUENCE_ID, float(_seq_id));
            _sensor.set_option(_option, _value);
        }
        ImGui::End();
    }

public:
    rs2::sensor& _sensor;
    rs2_option _option;
    rs2::option_range _range;
    //flags for the sliders
    const static int _sliders_flags = ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBringToFrontOnFocus;
};

//text box for ImGui
class text_box {
public:
    text_box(const char* name, ImVec2 position, ImVec2 size) : _name(name), _position(position), _size(size) {}

    void show(const char* text)
    {
        ImGui::SetNextWindowSize(_size);
        ImGui::SetNextWindowPos(_position);
        ImGui::Begin(_name, nullptr, _text_box_flags);
        ImGui::Text("%s",text);

        ImGui::End();
    }
    void remove_title_bar() {
        _text_box_flags |= ImGuiWindowFlags_NoTitleBar;
    }

public:
    const char* _name;
    ImVec2 _position;
    ImVec2 _size;
    // flags for displaying text box
    int _text_box_flags = ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoFocusOnAppearing
        | ImGuiWindowFlags_AlwaysUseWindowPadding
        | ImGuiWindowFlags_NoBringToFrontOnFocus
        | ImGuiWindowFlags_AlwaysAutoResize;
};


//input select 









class hdr_widgets {
public:
    // c'tor that creats all 4 sliders and text boxes
    // needed to init in an init list because no default c'tor for sliders and they are allocated inside hdr_widgets
    hdr_widgets(rs2::depth_sensor& depth_sensor):
        _exposure_slider_seq_1("Exposure", 1, 8000,
            depth_sensor, RS2_OPTION_EXPOSURE, depth_sensor.get_option_range(RS2_OPTION_EXPOSURE), { 130, 180 }, { 350, 40 }),
        _exposure_slider_seq_2("Exposure", 2, 18,
            depth_sensor, RS2_OPTION_EXPOSURE, depth_sensor.get_option_range(RS2_OPTION_EXPOSURE), { 390, 180 }, { 350, 40 }),
        _gain_slider_seq_1("Gain", 1, 25,
            depth_sensor, RS2_OPTION_GAIN, depth_sensor.get_option_range(RS2_OPTION_GAIN), { 130, 220 }, { 350, 40 }),
        _gain_slider_seq_2("Gain", 2, 16,
            depth_sensor, RS2_OPTION_GAIN, depth_sensor.get_option_range(RS2_OPTION_GAIN), { 390, 220 }, { 350, 40 }),
        _text_box_hdr_explain("HDR Tutorial", { 120, 20 }, { 1000, 140 }),
        _text_box_first_frame("frame 1", { 200, 150 }, { 170, 40 }),
        _text_box_second_frame("frame 2", { 460, 150 }, { 170, 40 }),
        _text_box_hdr_frame("hdr", { 850, 280 }, { 170, 40 })
    {
        // init frames map
        //for initilize only - an empty frame with its properties
        rs2::frame frame;

        //set each frame with its properties:
        //  { tile's x coordinate, tiles's y coordinate, tile's width (in tiles), tile's height (in tiles), priority (default value=0) }, (x=0,y=0) <-> left bottom corner
        //priority sets the order of drawing frame when two frames share part of the same tile, 
        //meaning if there are two frames: frame1 with priority=-1 and frame2 with priority=0, both with { 0,0,1,1 } as property,
        //frame2 will be drawn on top of frame1
        _frames_map[IR1] = frame_and_tile_property(frame, { 0,0,1,1,Priority::high });
        _frames_map[IR2] = frame_and_tile_property(frame, { 1,0,1,1,Priority::high });
        _frames_map[DEPTH1] = frame_and_tile_property(frame,{ 0,1,1,1,Priority::high });
        _frames_map[DEPTH2] = frame_and_tile_property(frame, { 1,1,1,1,Priority::high });
        _frames_map[HDR] = frame_and_tile_property(frame, { 2,0,2,2,Priority::high });
    }

    //show the features of the ImGui we have created
    //we need slider 2 to be showen before slider 1 (otherwise slider 1 padding is covering slider 2)
    void render_widgets() {

        //start a new frame of ImGui
        ImGui_ImplGlfw_NewFrame(1);

        _exposure_slider_seq_2.show();
        _exposure_slider_seq_1.show();
        _gain_slider_seq_2.show();
        _gain_slider_seq_1.show();

        _text_box_first_frame.remove_title_bar();
        _text_box_first_frame.show("Sequence 1");

        _text_box_second_frame.remove_title_bar();
        _text_box_second_frame.show("Sequence 2");

        _text_box_hdr_frame.remove_title_bar();
        _text_box_hdr_frame.show("HDR Stream");
        _text_box_hdr_explain.show("CAMBIO This demo provides a quick overview of the High Dynamic Range (HDR) feature.\nThe HDR configures and operates on sequences of two frames configurations, for which separate exposure and gain values are defined.\nBoth configurations are streamed and the HDR feature uses both frames in order to provide the best depth image.\nChange the values of the sliders to see the impact on the HDR Depth Image.");

        //render the ImGui features: sliders and text
        ImGui::Render();

    }

    // return a reference to frames map
    frames_mosaic& get_frames_map() {
        return _frames_map;
    }

    void update_frames_map(const rs2::video_frame& infrared_frame, const rs2::frame& depth_frame, 
        const rs2::frame& hdr_frame, rs2_metadata_type hdr_seq_id, rs2_metadata_type hdr_seq_size) {

        // frame index in frames_map are according to hdr_seq_id and hdr_seq_size
        int infrared_index = int(hdr_seq_id);
        int depth_index = int(hdr_seq_id + hdr_seq_size);
        int hdr_index = int(hdr_seq_id + hdr_seq_size + 1);

        //work-around, 'get_frame_metadata' sometimes (after changing exposure or gain values) sets hdr_seq_size to 0 even though it 2 for few frames
        //so we update the frames only if hdr_seq_size > 0. (hdr_seq_size==0 <-> frame is invalid)
        if (hdr_seq_size > 0) {
            _frames_map[infrared_index].first = infrared_frame;
            _frames_map[depth_index].first = depth_frame;
            _frames_map[hdr_index].first = hdr_frame; //HDR shall be after IR1/2 & DEPTH1/2
        }
    }

public:

    frames_mosaic _frames_map;

    hdr_slider _exposure_slider_seq_1;
    hdr_slider _gain_slider_seq_1;
    hdr_slider _exposure_slider_seq_2;
    hdr_slider _gain_slider_seq_2;

    text_box _text_box_hdr_explain;
    text_box _text_box_first_frame;
    text_box _text_box_second_frame;
    text_box _text_box_hdr_frame;

    enum frame_id { IR1, IR2, DEPTH1, DEPTH2, HDR };

};



class custom_button {
public:
    custom_button(const char* label, ImVec2 position, ImVec2 size, float valueSlider0, float valueSlider1, float valueSlider2, float valueSlider3) 
        : _label(label), _position(position), _size(size), _valueSlider0(valueSlider0), _valueSlider1(valueSlider1), _valueSlider2(valueSlider2), _valueSlider3(valueSlider3) {}


    void show() {
        ImGui::SetNextWindowSize(_size);
        ImGui::SetNextWindowPos(_position);
        ImGui::Begin(_label, nullptr, _sliders_flags_2);


        if (ImGui::Button(_label, ImVec2(250, 50))) {
           std::cout << "Hola inge" << std::endl;
           std::cout << "Valor del slider 0: " << _valueSlider0 << std::endl;
        }

        ImGui::Text("\n"); // Espacio para mover el segundo botón a la siguiente línea
        
        if (ImGui::Button("Pausar proceso", ImVec2(250, 50))) {
            std::cout << "Hola inge (Botón 2)" << std::endl;
        }

        ImGui::Text("\n"); // Espacio para mover el segundo botón a la siguiente línea

        if (ImGui::Button("Detener proceso", ImVec2(250, 50))) {
            std::cout << "Hola inge (Botón 2)" << std::endl;
        }

    }

public:
    const char* _label;
    ImVec2 _position;
    ImVec2 _size;

    float _valueSlider0;
    float _valueSlider1;
    float _valueSlider2;
    float _valueSlider3;
    float _GAltitude;
    float _GAsimut;
    float _GRoll;

    const static int _sliders_flags_2= ImGuiWindowFlags_NoCollapse
        | ImGuiWindowFlags_NoScrollbar
        | ImGuiWindowFlags_NoSavedSettings
        | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoTitleBar
        | ImGuiWindowFlags_NoBringToFrontOnFocus;   
};


//Lista de funciones para el algoritmo de movimiento del robot 

int suma(int a, int b) {
    return a + b;
}


//Calcula los grados de los imputs 
void CalcularGrados(int Altitud, int Asimuth, int Roll ,int& GAltitude, int& GAsimut, int& GRoll ){
    double H[3];
    H[0]=Altitud;
    H[1]=Asimuth;
    H[2]=Roll;
    //Numero de grados se moveran por punto
    GAltitude=60/H[0];
    GAsimut=360/H[1];
    GRoll=90/H[2];
}

// En el siguiente arreglo almacena la distribuccion de los grados en altitude
std::vector<int>CalcularArreglo1(int GAltitude) {
    std::vector<int> array1;
    for (int i = 0; i <= 360; i += GAltitude) {
        array1.push_back(i);
    }
    return array1;
}

// En el siguiente arreglo almacena la distribuccion de los grados en Azimuth
std::vector<int>CalcularArreglo2(int GAsimut) {
    std::vector<int> array2;
    for (int i = 0; i <= 60; i += GAsimut) {
        array2.push_back(i);
    }
    return array2;
}

// En el siguiente arreglo almacena la distribuccion de los grados en Roll
std::vector<int>CalcularArreglo3(int GRoll) {
    std::vector<int> array3;
    for (int i = 0; i <= 90; i += GRoll) {
        array3.push_back(i);
    }
    return array3;
}

// En la siguiente funcion forma el los puntos donde se movera el robot
//inicializador de contador 
std::vector<std::vector<int>> CalcularPuntosMovimiento(const std::vector<int>& array1, const std::vector<int>& array2, int radio) {
    std::vector<std::vector<int>> S_data;

    for (int i = 0; i < array2.size(); ++i) {
        for (int j = 0; j < array1.size(); ++j) {
            std::vector<int> S = {array1[j], array2[i], radio};
            S_data.push_back(S);
        }
    }
    return S_data;
}

std::vector<std::vector<int>> CalcularPuntosCinematicaInversa(const std::vector<std::vector<int>>& result, int si) {
    std::vector<std::vector<int>> D; // Declara D fuera del bucle

    int n = 0;
    int l2=300; //Modificar l2 es para el tamaño del eslabon ingresarlo 
    while (n < si) {
        std::vector<int> temp = result[n]; // Asigna el vector de enteros a un vector temporal

        std::cout << "Contenido del vector temporal temp: ";
        for (int elem : temp) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;

       // Imprime los dos primeros elementos del vector temporal
        std::cout << "Contenido de temp en la iteracion Prueba " << n << ": ";
        std::cout << temp[0] << " " << temp[1] << std::endl;


        // En esta seccion se calculan los datos de grados a radianes 
        std::vector<double> rads=grados_a_radianes(temp);
        std::cout << "Contenido de rads:" << std::endl;
        for (double radian : rads) {
            std::cout << radian << " ";/////////////////En esta seccion separar cada elemento del contenido del vector  en q1 q2 q3
           
        }
        //Solo es prueba para inprimir los datos del vector rads 
        std::cout << std::endl;
        std::cout << "Contenido de rads por separado:" << std::endl;
        std::cout << rads[0] << " " << rads[1] << std::endl;
        std::cout << std::endl;

        //En esta seccion se calculara los datos de la funcion de cordenadas esfericas a cartecianas 
        auto result = sph2cart(rads[0],rads[1],temp[2]);
        //En esta seccion se imprime los datos de las cordenadas 
        double px = std::get<0>(result);
        double py = std::get<1>(result);
        double pz = std::get<2>(result);
        std::cout << "Coordenadas cartesianas (px, py, pz): ";
        std::cout << px << ", " << py << ", " << pz << std::endl;

        //En la siguiente seccion se va a calcular la cinematica inversa  
        //Para q1 se tiene que:
        auto q1=atan(-px/py);
        //Para q2 se tiene que:
        auto q2=(px*sin(q1)-py*cos(q1)+l2);
        //Para q3 se tiene que:
        auto q3=pz;
        //para q4 se tiene que:
        auto q4=atan(-q2/q3);
        //Impresion de las cordenadas de cada una de las cordenadas de la cinematica inversa 
        std::cout << "Coordenadas cartesianas (q1,q2,q3,q4): ";
        std::cout << q1 << ", " << q2 << ", " << q3 <<","<<q4<< std::endl;

        D.push_back(temp); // Agrega el vector temporal a D

        // Imprime el contenido de D en esta iteración
        std::cout << "Contenido de D en la iteracion " << n << ": ";
        for (const auto& elem : D.back()) {
            std::cout << elem << " ";
        }
        std::cout << std::endl;
        // Incrementa n para pasar a la siguiente fila
        n++;
    }
        
    return D;
}
//Funcion que convierte grados a radianes
std::vector<double> grados_a_radianes(const std::vector<int>& grados) {
    std::vector<double> radianes;
    radianes.reserve(grados.size()); // Reserva espacio para evitar reasignaciones

    for (int grado : grados) {
        double radian = grado * M_PI / 180.0;
        radianes.push_back(radian);
    }
    return radianes;
}

//Funcion para cordenadas Esfericas a cartecianas
std::tuple<double, double, double> sph2cart(double azimuth, double elevation, double radius) {
    double x = radius * std::cos(elevation) * std::cos(azimuth);
    double y = radius * std::cos(elevation) * std::sin(azimuth);
    double z = radius * std::sin(elevation);
    return std::make_tuple(x, y, z);
}



