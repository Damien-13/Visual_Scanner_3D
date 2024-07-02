#include "Exporter.h"
#include <filesystem>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/IO/STL.h>
#include <fstream>
#include <vector>
#include <windows.h>
#include <commdlg.h>
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polyhedron_3<K> Polyhedron;
typedef K::Point_3 Point_3;
typedef std::vector<std::size_t> Facet;

Exporter::Exporter(){
}

void Exporter::ExporterSTL(std::vector<float>* vertices) {
    std::vector<Point_3> points;
    std::vector<Facet> facets;

    // Remplir les points à partir du vecteur de sommets
    for (int i = 0; i < vertices->size(); i += 3) {
        points.push_back(Point_3((*vertices)[i], (*vertices)[i + 1], (*vertices)[i + 2]));
    }

    // Remplir les facettes (chaque trois points forment une facette)
    for (int i = 0; i < points.size(); i += 3) {
        Facet facet;
        facet.push_back(i);
        facet.push_back(i + 1);
        facet.push_back(i + 2);
        facets.push_back(facet);
    }

    // Ouvrir la boîte de dialogue de sauvegarde de fichiers
    OPENFILENAME ofn;
    char filename[MAX_PATH] = "";
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Fichiers STL\0*.stl\0";
    wchar_t wfilename[MAX_PATH] = L"";
    MultiByteToWideChar(CP_ACP, 0, filename, -1, wfilename, MAX_PATH);
    ofn.lpstrFile = wfilename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Enregistrer sous";
    ofn.Flags = OFN_OVERWRITEPROMPT;

    if (GetSaveFileName(&ofn)) {
        char filename[MAX_PATH];
        WideCharToMultiByte(CP_ACP, 0, wfilename, -1, filename, MAX_PATH, NULL, NULL);

        // Vérifier si l'extension .stl est présente
        std::string strFilename(filename);
        if (strFilename.substr(strFilename.find_last_of(".") + 1) != "stl") {
            strFilename += ".STL";
        }

        std::ofstream out(strFilename);
        if (!out) {
            std::cerr << "Impossible d'ouvrir le fichier STL pour l'écriture." << std::endl;
            return;
        }
        CGAL::IO::write_STL(out, points, facets);
        out.close();
    }

    
}


void Exporter::ExportBlender() {

}