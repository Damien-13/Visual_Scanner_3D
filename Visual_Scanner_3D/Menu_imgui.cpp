#include "Menu_imgui.h"
#include <boost/asio.hpp>

Menu_imgui::Menu_imgui(GLFWwindow* window) {

    // Cr�ation du contexte ImGui
    //cout << "test begin imgui" << endl;
    IMGUI_CHECKVERSION();
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
    //cout << "end test imgui" << endl;
    

    //boost::asio::io_service io_service;
    socket = new boost::asio::ip::tcp::socket(io_service);
    socket->connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(server_ip), server_port));
    
}

void Menu_imgui::set_data(std::vector<float>* datavertices, TriangleConstructeur &objet3DIN) {
    vertices = datavertices;
    objet3D = &objet3DIN;
}

void  Menu_imgui::Menu_run()
{
    

    //------------------------------------------------------------------------------------------------------------------bar du menu 
    ImGui::BeginMainMenuBar();
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.584f, 0.635f, 0.729f, 1.0f)); // D�finit la couleur d'arri�re-plan du menu
    ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.584f, 0.635f, 0.729f, 1.0f)); // D�finit la couleur d'arri�re-plan du menu
    if (ImGui::BeginMenu("File")) {
        if (ImGui::BeginMenu("Export to")) {
            if (ImGui::MenuItem("STL")) {//(ImGui::MenuItem("STL", "Ctrl+S+T+L")
                ExporterSTL();
            }

            if (ImGui::MenuItem("Blender")) {
                ExportBlender();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Scanner")) {
        if (ImGui::MenuItem("Start")) {
            StartToScan();
        }
        if (ImGui::MenuItem("Stop")) {
            StopToScan();
        }
        if (ImGui::MenuItem("Object Display")) {
            objet3D->Preparation_donnee();
        }
       


        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Help")) {
        if (ImGui::MenuItem("About")) {


            About = TRUE;
            //Mettre le code pour aller � propos 
        }
        if (ImGui::MenuItem("Doc")) {
            //Mettre le code pour la doc 
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if (About == TRUE) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.584f, 0.635f, 0.729f, 1.0f));
        ImGui::BeginPopupModal("About");
        ImGui::Text("App developed by Damien Molinieres\n\n\nScanner designed by Leopold Aron and Clement Michel");
        if (ImGui::Button("ok")) {
            ImGui::CloseCurrentPopup();
            std::cout << "L'utilisateur a cliqu� sur OK, fermeture de la popup About..." << std::endl;
            About = FALSE;
        }
        ImGui::EndPopup();
    }

    if (Start_pop_up == TRUE) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.584f, 0.635f, 0.729f, 1.0f));
        ImGui::BeginPopupModal("Start");

        ImGui::Text("The scan is in progress, please wait");

        if (ImGui::Button("ok")) {
            ImGui::CloseCurrentPopup();
            std::cout << "L'utilisateur a cliqu� sur OK, fermeture de la popup ..." << std::endl;
            Start_pop_up = FALSE;
        }
        ImGui::EndPopup();
    }
    if (Start_pop_up_eror == TRUE) {
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.584f, 0.635f, 0.729f, 1.0f));
        ImGui::BeginPopupModal("Start");

        ImGui::Text("Error communicating with server, please try again");

        if (ImGui::Button("ok")) {
            ImGui::CloseCurrentPopup();
            std::cout << "L'utilisateur a cliqu� sur OK, fermeture de la popup ..." << std::endl;
            Start_pop_up_eror = FALSE;
        }
        ImGui::EndPopup();
    }

    /*   ImGui::Begin("test");
       ImGui::Text("ouraaaaa!!!");
       ImGui::End();*/
       //----------------------------------------------------------------------------------------------------------------------
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   
}
bool Menu_imgui::POP_UP_run() {
    
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        

        ImGui::BeginPopupModal("Start set");
        std::cout << "Affichage de la popup..." << std::endl;
        ImGui::Text("Veuillez vous connecter au WiFi !");
        if (ImGui::Button("connected")) {
            ImGui::CloseCurrentPopup();
            std::cout << "L'utilisateur a cliqu� sur OK, fermeture de la popup..." << std::endl;
            return 1;
        }
        ImGui::EndPopup();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}

void Menu_imgui::ExporterSTL() {
    // Code pour exporter en STL
    //cout << "exporter en STL " << endl;
    object_exporte.ExporterSTL(vertices);
}

void Menu_imgui::ExportBlender() {
    object_exporte.ExporterSTL(vertices);
    // Code pour exporter en Blender
   // cout << "exporter en Blender " << endl;
}

void Menu_imgui::StartToScan() {
    //const std::string server_ip = "192.168.4.1";
    //const int server_port = 10000; // Remplacez par le port du serveur
    start = true;
    message = "continue";
    try {
        Start_pop_up =TRUE ;
        //boost::asio::io_service io_service;

        //boost::asio::ip::tcp::socket socket(io_service);
        //socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(server_ip), server_port));
          
        const std::string msg = "start";
        boost::asio::write(*socket, boost::asio::buffer(msg));
        std::cout << "Message envoye au serveur. En attente de la reponse..." << std::endl;
        
        
        
      
       
    }
    catch (std::exception& e) {
         Start_pop_up = FALSE;
         reponse = FALSE;
        std::cerr << e.what() << std::endl;
        Start_pop_up_eror = TRUE;
    }
    
}


void Menu_imgui::StopToScan() {
    message = "stop";
    Start_pop_up = TRUE;
    reponse = FALSE;
    start = true;
    try {
        
        //boost::asio::io_service io_service;

        //boost::asio::ip::tcp::socket socket(io_service);
        //socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(server_ip), server_port));

        const std::string msg = "stop";
        boost::asio::write(*socket, boost::asio::buffer(msg));
        std::cout << "Message envoye au serveur. En attente de la reponse..." << std::endl;





    }
    catch (std::exception& e) {
        Start_pop_up = FALSE;
        reponse = FALSE;
        std::cerr << e.what() << std::endl;
        Start_pop_up_eror = TRUE;
    }
        
}

void Menu_imgui::listenForOrder() 
{
    if (start == true) {
        //const std::string server_ip = "192.168.4.1";
        //const int server_port = 10000; // Remplacez par le port du serveur
        reponse = FALSE;


        //boost::asio::io_service io_service;
        Start_pop_up = FALSE;
        //boost::asio::ip::tcp::socket socket(io_service);
        //socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(server_ip), server_port));
        // Cr�er un tampon pour stocker la r�ponse
        boost::asio::streambuf response;

        // Lire la r�ponse du serveur
        //std::cout << "read" << std::endl;
        boost::asio::read_until(*socket, response, "\n");

        // Convertir le tampon en string
        std::string response_str(boost::asio::buffer_cast<const char*>(response.data()));

        if (response_str == "order\n") {
            boost::asio::write(*socket, boost::asio::buffer(message));

        }


    }
}