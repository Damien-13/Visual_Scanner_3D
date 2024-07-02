#include "TriangleConstructeur.h"
#include "SQLAPI.h" //bibliothèque de Connexion à la base de données 
#pragma comment(lib,"version")





TriangleConstructeur::TriangleConstructeur(float xIN, float yIN, float zIN, float sizeIN)
{
     x= xIN;
     y= yIN;
     z=zIN;
     size= sizeIN;
}




void  TriangleConstructeur::Preparation_donnee() {

    SAConnection con; // create a connection object
    copyvertices = new std::vector<float>;
    vertices = new std::vector<float>;
    verticesExporte = new std::vector<float>;
    // connect to database (MariaDB)

    try {
        con.Connect("192.168.4.1@DATAPOINT", "appscan", "Scanner3D", SA_MariaDB_Client);
        //con.Connect("127.0.0.1@DATAPOINT", "appscan", "Scanner3D", SA_MariaDB_Client);

        SACommand cmd(&con, "SELECT X, Y, Z, R, G, B FROM POINT WHERE NUM_OBJET = 1");
        cmd.Execute();
        while (cmd.FetchNext()) {
            
            std::cout << cmd.Field("X").asDouble() << ' ';
            std::cout << cmd.Field("Y").asDouble() << ' ';
            std::cout << cmd.Field("Z").asDouble() << ' ';
            std::cout << cmd.Field("R").asDouble() << ' ';
            std::cout << cmd.Field("G").asDouble() << ' ';
            std::cout << cmd.Field("B").asDouble() << ' ';
            std::cout << std::endl;
            
            copyvertices->push_back(cmd.Field("X").asDouble());
            copyvertices->push_back(cmd.Field("Y").asDouble());
            copyvertices->push_back(cmd.Field("Z").asDouble());
            copyvertices->push_back(cmd.Field("R").asDouble());
            copyvertices->push_back(cmd.Field("G").asDouble());
            copyvertices->push_back(cmd.Field("B").asDouble());


            //vertices->push_back(cmd.Field("X").asDouble());
            //vertices->push_back(cmd.Field("Y").asDouble());
            //vertices->push_back(cmd.Field("Z").asDouble());
            //vertices->push_back(cmd.Field("R").asDouble());
            //vertices->push_back(cmd.Field("G").asDouble());
            //vertices->push_back(cmd.Field("B").asDouble());

        }
    }
    catch (const SAException& e) {
        // Gérer l'exception ici
        std::cerr << e.ErrText().GetMultiByteChars();
    }

    reconstituerTriangles(copyvertices, vertices,verticesExporte);

    for (unsigned long i = 2; i < vertices->size(); i += 6) {
        vertices->at(i) = (vertices->at(i) * size) + z;
        vertices->at(i - 1) = (vertices->at(i - 1) * size) + y;
        vertices->at(i - 2) = (vertices->at(i - 2) * size) + x;
    }
    std::cout << "size\n";
}


// Fonction pour reconstituer des triangles
void TriangleConstructeur::reconstituerTriangles(std::vector<float>* copyvertices, std::vector<float>* vertices,std::vector<float>* verticesExporte) {
    
    std::vector<std::pair<Point, unsigned>> pointsWithInfo;
    std::vector<std::tuple<float, float, float>> colors; // Stocker les couleurs
   
    // Convertir les données d'entrée en points pour la triangulation et stocker les couleurs
    for (size_t i = 0; i < copyvertices->size(); i += 6) {
        Point point((*copyvertices)[i], (*copyvertices)[i + 1], (*copyvertices)[i + 2]);
        pointsWithInfo.push_back(std::make_pair(point, i / 6)); // Associer chaque point à son index
        colors.emplace_back((*copyvertices)[i + 3], (*copyvertices)[i + 4], (*copyvertices)[i + 5]); // Stocker la couleur
    }
    
    // Effectuer la triangulation de Delaunay
    Delaunay dt(pointsWithInfo.begin(), pointsWithInfo.end());
    
    vertices->clear(); // S'assurer que le vecteur de sortie est vide
    verticesExporte->clear();
    // Parcourir les cellules (tetrahedra) de la triangulation

    std::cout << "tfit\n";
    for (auto fit = dt.finite_cells_begin(); fit != dt.finite_cells_end(); ++fit) {
        
        for (int i = 0; i < 4; i++) { // Pour chaque sommet de la cellule
            Delaunay::Vertex_handle vertex = fit->vertex(i);
            if (dt.is_infinite(vertex)) continue; // Ignorer les sommets infinis
            unsigned index = vertex->info();
            Point p = vertex->point();

            // Ajouter les coordonnées du point
            vertices->push_back(CGAL::to_double(p.x()));
            vertices->push_back(CGAL::to_double(p.y()));
            vertices->push_back(CGAL::to_double(p.z()));
            verticesExporte->push_back(CGAL::to_double(p.x()));
            verticesExporte->push_back(CGAL::to_double(p.y()));
            verticesExporte->push_back(CGAL::to_double(p.z()));

            // Ajouter la couleur associée au point
            auto color = colors[index];
            vertices->push_back(std::get<0>(color)); // r
            vertices->push_back(std::get<1>(color)); // g
            vertices->push_back(std::get<2>(color)); // b
            verticesExporte->push_back(std::get<0>(color)); // r
            verticesExporte->push_back(std::get<1>(color)); // g
            verticesExporte->push_back(std::get<2>(color)); // b
        }
        std::cout << "tloop\n";
    }
    std::cout << "t6\n";
}


std::vector<float>* TriangleConstructeur::getVertice()
{
    return vertices;
}
std::vector<float>* TriangleConstructeur::getVerticeExporte()
{
    return verticesExporte;
}
