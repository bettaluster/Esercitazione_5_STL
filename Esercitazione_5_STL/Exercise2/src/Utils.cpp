#include "Utils.hpp"
#include "UCDUtilities.hpp"
#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>


namespace PolygonalLibrary

{
bool ImportMesh(PolygonalMesh& mesh)
    {
    unsigned int i=0;
    if(ImportCell0Ds(mesh)){i++;}

    if(ImportCell1Ds(mesh)){i++;}

    if(ImportCell2Ds(mesh)){i++;}

    if (i==3)
        {
        return true;
        }
    else
        {
        return false;
        }
    }

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file_Cell0Ds("./Cell0Ds.csv");
    if (!file_Cell0Ds.is_open())
    {
        cerr << "Error opening file Cell0Ds.csv" << endl;
        return false;
    }
    string riga;
    list<string> lista_dim;
        while (getline(file_Cell0Ds, riga))
    {
        lista_dim.push_back(riga);
    }
    file_Cell0Ds.close();
    lista_dim.pop_front();//tolgo l'intestazione
    mesh.Dim0D = lista_dim.size();

    if (mesh.Dim0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    mesh.M0D = Eigen::MatrixXd::Zero(3, mesh.Dim0D); 
// voglio stampare la lista
    for (string& li : lista_dim) // itero una lista
    {
        replace(li.begin(), li.end(), ';', ' ');
        istringstream convertitore(li);
        unsigned int id,marker;
        convertitore >> id >> marker >> mesh.M0D(0,id) >> mesh.M0D(1,id) ;
    if (marker != 0)
    {
        auto it = mesh.marker0D.find(marker);
        if (it != mesh.marker0D.end())//verrifico se il marker è già presente // se metto == campio if con else
              (*it).second.push_back(id);//memoriazza gli id nei marker gia presenti
        else
            mesh.marker0D.insert({marker, {id}});//memoriazza gli id nei marker
        }

    }
return true;
}

bool ImportCell1Ds(PolygonalMesh& mesh)
    {
        {
            ifstream file_Cell1Ds("./Cell1Ds.csv");
            if (!file_Cell1Ds.is_open())
            {
                cerr << "Error opening file Cell0Ds.csv" << endl;
                return false;
            }
            string riga;
            list<string> lista_dim;
            while (getline(file_Cell1Ds, riga))
            {
                lista_dim.push_back(riga);
            }
            file_Cell1Ds.close();
            lista_dim.pop_front();//tolgo l'intestazione
            mesh.Dim1D = lista_dim.size();
            if (mesh.Dim1D == 0)
            {
                cerr << "There is no cell 1D" << endl;
                return false;
            }
            mesh.M1D = MatrixXi::Zero(2, mesh.Dim1D); ; 
            for (string& li : lista_dim) // itero una lista
            {
              replace(li.begin(), li.end(), ';', ' ');
              istringstream convertitore(li);
              unsigned int id,marker;
              convertitore >> id >> marker >> mesh.M1D(0,id)>> mesh.M1D(1,id); 
              if (marker != 0)
              {
                auto it = mesh.marker1D.find(marker);
                if (it != mesh.marker1D.end())// 
                    (*it).second.push_back(id);//memoriazza gli id nei marker
                else
                    mesh.marker1D.insert({marker, {id}});//memoriazza g
              }
            } 
        }
        return true;
    }   

bool ImportCell2Ds(PolygonalMesh& mesh)
    {
        ifstream file_Cell2Ds("./Cell2Ds.csv");
        if (!file_Cell2Ds.is_open())
        {
            cerr << "Error opening file Cell2Ds.csv" << endl;
            return false;
        }
        string riga;
        list<string> lista_dim;
        while (getline(file_Cell2Ds, riga))
        {
            lista_dim.push_back(riga);
        }
        file_Cell2Ds.close();
        lista_dim.pop_front();//tolgo l'intestazione
        mesh.Dim2D = lista_dim.size();
        if (mesh.Dim2D == 0)
        {
            cerr << "There is no cell 2D" << endl;
            return false;
        }

        for (string& li : lista_dim) // itero una lista
        
        {
          replace(li.begin(), li.end(), ';', ' ');
          istringstream convertitore(li);
          unsigned int id,marker,n_vertici_spigoli;
          convertitore >> id >> marker >> n_vertici_spigoli;
          if (marker != 0)
              {
                auto it = mesh.marker2D.find(marker);
                if (it != mesh.marker2D.end())// 
                    (*it).second.push_back(id);//memoriazza gli id nei marker
                else
                    mesh.marker2D.insert({marker, {id}});//memoriazza g
              }
// lavoro su matrice vertici
          mesh.M2D_vertici.reserve(mesh.Dim2D);
          vector<int> linea1;
          linea1.resize(n_vertici_spigoli);// uso resize per allocare la memoria
          for (unsigned int j = 0; j < n_vertici_spigoli ; j++)
          {
            convertitore >> linea1[j];
          }
          mesh.M2D_vertici.push_back(linea1);
          
        
          // scarto numero spigoli         
          convertitore >> n_vertici_spigoli;

// lavoro su matrice spigoli
          mesh.M2D_spigoli.reserve(mesh.Dim2D);    
          vector<int> linea2;
          linea2.reserve(n_vertici_spigoli);
          for (unsigned int j = 0; j < n_vertici_spigoli ; j++)
          {
            convertitore >> linea2[j];
          }

          mesh.M2D_spigoli.push_back(linea2);
          
        }
        
    return true;
    }

bool controllo_lunghezza(PolygonalMesh& mesh){

    for (unsigned int i = 0; i < mesh.Dim1D; i++)
    {
        unsigned int vertice1 = mesh.M1D(0,i);
        unsigned int vertice2 = mesh.M1D(1,i);
        double coordinataX_vertice1 = mesh.M0D(0,vertice1);
        double coordinataY_vertice1 = mesh.M0D(1,vertice1);
        double coordinataX_vertice2 = mesh.M0D(0,vertice2);
        double coordinataY_vertice2 = mesh.M0D(1,vertice2);
        double lunghezza = sqrt(pow(coordinataX_vertice2 - coordinataX_vertice1, 2) +
                                 pow(coordinataY_vertice2 - coordinataY_vertice1, 2));
        double epsilon = 1e-6; // soglia di tolleranza
        if (lunghezza < epsilon)
        {
            return false;
        }
    }
    return true;

}

bool controllo_area(PolygonalMesh& mesh){

    {
        for (unsigned int j = 0; j < mesh.Dim2D; j++)
        {
        double Area=0;
            for (unsigned int i = 0; i <  mesh.M2D_vertici[j].size() ; i ++)
            {   
       
                int indice_vertice1 = mesh.M2D_vertici[j][i];
                int indice_vertice2;
                if (i + 1 == mesh.M2D_vertici[j].size())
                {
                    indice_vertice2 = mesh.M2D_vertici[j][0];
                }
                else
                {
                    indice_vertice2 = mesh.M2D_vertici[j][i+1];
                }
                double vertice1_x = mesh.M0D(0,indice_vertice1);
                double vertice1_y = mesh.M0D(1,indice_vertice1);
                double vertice2_x = mesh.M0D(0,indice_vertice2);
                double vertice2_y = mesh.M0D(1,indice_vertice2);
                Area += (vertice1_x*vertice2_y - vertice2_x*vertice1_y);
            }
        Area = abs(Area)/2;
        double epsilon = 1e-8; // soglia di tolleranza
        if (Area < epsilon)
        {
        return false;
        }
        }
        
    
    return true;
    
    }

}

}


