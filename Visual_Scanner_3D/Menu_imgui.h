#pragma once

#include "TriangleConstructeur.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>
#define GLFW_INCLUDE_DIR
#include <GLFW/glfw3.h>
#include <linmath.h>
#include <boost/asio.hpp>
#include "Exporter.h"


#ifndef Menu_imguiClass
#define Menu_imguiClass

class Menu_imgui
{

public:
	bool start = 0;
	bool About=0;
	bool reponse = 0;
	bool Start_pop_up = 0;
	bool Start_loding=0;
	bool Start_pop_up_eror;
	std::vector<float>* vertices;
	Exporter object_exporte;
	bool stateExporterSTL=0;
	bool stateExportBlender = 0;
	bool stateStartToScan = 0;
	bool stateStopToScan = 0;
	Menu_imgui(GLFWwindow* window);
	void Menu_run();
	bool POP_UP_run();
	TriangleConstructeur* objet3D = nullptr;
	void set_data(std::vector<float>* datavertices, TriangleConstructeur &objet3DIN);
	void listenForOrder();
private:
	std::string message = "";
	void ExporterSTL();
	void ExportBlender();
	void StartToScan();
	void StopToScan();

	const std::string server_ip = "192.168.4.1";
	const int server_port = 10000; // Remplacez par le port du serveur

	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket* socket;

};
#endif
