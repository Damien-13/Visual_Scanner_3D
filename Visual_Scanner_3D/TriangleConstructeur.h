#pragma once
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_3<unsigned, K> Vb;
typedef CGAL::Triangulation_data_structure_3<Vb> Tds;
typedef CGAL::Delaunay_triangulation_3<K, Tds> Delaunay;
typedef K::Point_3 Point;


#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/Vertices.h>
#include <pcl/PolygonMesh.h>
//#include <pcl/io/pcd_io.h>
#include <pcl/conversions.h>
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <linmath.h>

class TriangleConstructeur
{
	private:
		// initialazed in constructor
		//static int width; 
		//static int height; 
		
		// model main var
		//float pos[3] = { 0.0, 0.0, 0.0 };
		//float size = 1.0;

		// model structure bone
		//static const char* vertexShaderSource;
		//static const char* fragmentShaderSource;
		std::vector<float>* vertices;
		std::vector<float>* verticesExporte;
		std::vector<float>* copyvertices;
		// shader compiling
		//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		//unsigned int shaderProgram = glCreateProgram();
		// linking vertex attributes
		//unsigned int VAO = 0; 
		//unsigned int VBO = 0;
		float x;
		float y; 
		float z; 
		float size;

		void  reconstituerTriangles(std::vector<float>* copyvertices, std::vector<float>* vertices, std::vector<float>* verticesExporte);
		
	public:
		TriangleConstructeur(float x, float y, float z, float size);
		void  Preparation_donnee();
		//void setPos(float x, float y, float z);
		//void setRot(float x, float y, float z);
		//void setSize(float size);

		std::vector<float>* getVertice();
		std::vector<float>* getVerticeExporte();
};

