#pragma once
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/STL.h>
#include <fstream>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef K::Point_3 Point_3;
typedef std::vector<std::size_t> Facet;


#ifndef ExporterClass
#define ExporterClass
class Exporter
{

public:
	Exporter();
	void ExporterSTL(std::vector<float>* vertices);
	void ExportBlender();
	
};

#endif