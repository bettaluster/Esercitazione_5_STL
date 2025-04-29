#include <iostream>
#include "PolygonalMesh.hpp"
#include "Utils.hpp"
#include "UCDUtilities.hpp"

using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;


int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        cerr << "file not found" << endl;
        return 1;
    }
    else
    {
        cout << "File imported successfully" << endl;
    }

  


    
    Gedim::UCDUtilities utilities;
    utilities.ExportPoints("./Cell0Ds.inp",
                            mesh.M0D);
    utilities.ExportSegments("./Cell1Ds.inp",
                              mesh.M0D,
                              mesh.M1D);
                              
    if (!controllo_lungheza(mesh))
    {
        cerr << "Error in the length of the mesh" << endl;
        return 1;
    }
    else
    {
        cout << "Length check passed" << endl;
    }


    
    if (!controllo_area(mesh))
    {
        cerr << "Error in the area of the mesh" << endl;
        return 1;
    }
    else
    {
        cout << "Area check passed" << endl;
    }
    

return 0;

}



