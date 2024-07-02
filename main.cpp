//------------------------------------ imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//--------------------------------------------
#include "Menu_imgui.h"
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <glad/glad.h>
#define GLFW_INCLUDE_DIR
#include <GLFW/glfw3.h>
#include <linmath.h>
#include "TriangleConstructeur.h"




//#include "lib3mf_implicit.hpp"---------STL

//-------------------------------Icône de L'application de la bibliothèque 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//---------------------------------------

using namespace std;
int width, height;
mat4x4 m, p, mvp;
float x = 0;
float y = 0;
float z = 0;


// special variable and shaders sources codes
const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"uniform mat4 transform;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0);\n"
"}\0";



static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    //rotation
    if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT || key == GLFW_KEY_UP || key == GLFW_KEY_DOWN || key == GLFW_KEY_Z || key == GLFW_KEY_S) {
        float ratio = width / (float)height;

        mat4x4_identity(m);
        switch (key) {
        case GLFW_KEY_LEFT:
            y += -0.005f;
            break;
        case GLFW_KEY_RIGHT:
            y += 0.005f;
            break;
        case GLFW_KEY_UP:
            x += -0.005f;
            break;
        case GLFW_KEY_DOWN:
            x += 0.005f;
            break;
        case GLFW_KEY_Z:
            z += -0.05f;
            break;
        case GLFW_KEY_S:
            z += 0.05f;
            break;
        }
        mat4x4_rotate_X(m, m, x);
        mat4x4_rotate_Y(m, m, y);
        mat4x4_rotate_Z(m, m, z);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
    }
}


int main(void)
{
    char infoLog[512];
    int success = 0;
 

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 960, "Visual Scanner 3D", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Load the image
    int iconwidth = 64;
    int iconheight = 64;
    int nrChannels = 0;
    unsigned char* data = stbi_load("logo.png", &iconwidth, &iconheight, &nrChannels, 0);

    // Set the window icon
    if (data) {
        GLFWimage images[1];
        images[0].width = iconwidth;
        images[0].height = iconheight;
        images[0].pixels = data;
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(data);
    }

  
    Menu_imgui menu(window);


    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);


    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);
    //--------------------------------------------------Création des points 

    //menu.POP_UP_run();
   
    
    
    /*
    bool afficherPopup = false;
    bool etat_de_la_connexion = 0;
    SAConnection con; // create a connection object
    bool messageAffiche = false;

    //Création du contexte 
    IMGUI_CHECKVERSION();
    glfwMakeContextCurrent(window);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    std::cout << "Initialisation terminée, tentative de connexion à la base de données..." << std::endl;

    while (etat_de_la_connexion == 0) {
        
            std::cout << "Tentative de connexion à la base de données..." << std::endl;
            con.Connect("192.168.1.62@DATAPOINT", "appscan", "Scanner3D", SA_MariaDB_Client);

            std::cout << "Connexion réussie, exécution de la requête SQL..." << std::endl;
            SACommand cmd(&con, "SELECT X, Y, Z, R, G, B FROM POINT WHERE NUM_OBJET = 1");
            cmd.Execute();

            if (cmd.FetchNext() == 1) {
                std::cout << "La requête SQL a renvoyé des résultats, mise à jour de l'état de la connexion..." << std::endl;
                etat_de_la_connexion = 1; //On a réussi à se connecter
            }
            else {
                std::cout << "La requête SQL n'a pas renvoyé de résultats, affichage de la popup..." << std::endl;
                afficherPopup = true;
            }
        
            try {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }
            catch (exception e) {
                std::cout << "err: " << endl << e.what() << endl;
            }

        if (afficherPopup) {
            
            ImGui::BeginPopupModal("MaPopup");
            std::cout << "Affichage de la popup..." << std::endl;
            ImGui::Text("Veuillez vous connecter au WiFi !");
            if (ImGui::Button("OK")) {
                ImGui::CloseCurrentPopup();
                std::cout << "L'utilisateur a cliqué sur OK, fermeture de la popup..." << std::endl;
            }
            ImGui::EndPopup();
        }
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    */
    TriangleConstructeur objet3D(0.0f, 0.0f, 0.0f, 0.1f);
        objet3D.Preparation_donnee();
        std::cout << objet3D.getVertice()->size() << std::endl;
        menu.set_data(objet3D.getVerticeExporte(),objet3D);
        //point test1(0.0f, -0.25, 0.5f, 0.5f);
        std::cout << "t7\n";
         //std::vector<float> test = test0.getVertice();
         //std::vector<float> test2 = test1.getVertice();


        //std::vector<float> test;

        for (unsigned long i = 2; i < objet3D.getVertice()->size(); i+=6) {
            cout << "x: " << objet3D.getVertice()->at(i-2) << endl;
            cout << "y: " << objet3D.getVertice()->at(i - 1) << endl;
            cout << "z: " << objet3D.getVertice()->at(i) << endl;
        }



        /* -- shader -- */
        // vertex shader-----------------------------------------------------------------------------------------------------------------------------
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        //check if the shader was successfullt compiled
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (success == false) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << infoLog << std::endl;
        }
        std::cout << "vertex shader\n";
        // fragment shader------------------------------------------------------------------------------------------------------------------------------
        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);
        //check if the shader was successfullt compiled
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (success == false) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << infoLog << std::endl;
        }
        std::cout << "fragment shader\n";
        // shader program-----------------------------------------------------------------------------------------------------------------------
        unsigned int shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        //check if the shader was successfullt compiled
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success == false) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED: " << infoLog << std::endl;
        }
        std::cout << "shader program\n";
        //----------------------------------------------------------------------------------------------------------------------------------------------------------
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        /* -- linking vertex attributes -- */
        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        //bind Vertex Array Object
        glBindVertexArray(VAO);
        std::cout << "t8\n";
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        std::cout << "t8.5\n";
        glBufferData(GL_ARRAY_BUFFER, objet3D.getVertice()->size() * sizeof(float), &objet3D.getVertice()->front(), GL_STATIC_DRAW);
        //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        std::cout << "t9\n";
        //position attributes
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        GLint trans_location = glGetUniformLocation(shaderProgram, "transform"); //used for rotation

        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//draw in wireframe polygons


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        //render without the need of touching a key
        float ratio = width / (float)height;
        mat4x4_identity(m);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        
        std::cout << "entering while loop " << endl;
        while (!glfwWindowShouldClose(window))
        {
            int windowidth = 0, windoheight = 0;
            glfwGetWindowSize(window, &width, &height);
            /* -- drawing -- */
            //clear the colorbuffer
            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


            //----------------------------------------------------------------------
             //cout << "tet imgui new frame" << endl;
            try {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
            }
            catch (exception e) {
                std::cout << "err: " << endl << e.what() << endl;
            }
            //-----------------------------------------------------------------------
            //render the triangle
            // cout << "rendering" << endl;
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            glUniformMatrix4fv(trans_location, 1, GL_FALSE, (const GLfloat*)mvp);

            glDrawArrays(GL_TRIANGLES, 0, objet3D.getVertice()->size() / 6);
            //cout << "menu_run" << endl;
            menu.Menu_run();
            //cout << "end menu_run" << endl;

            /* -- buffer -- */
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(shaderProgram);

        glfwTerminate();
        exit(EXIT_SUCCESS);
    
    return 0;
}

