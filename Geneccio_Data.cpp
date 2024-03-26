#include <iostream>
#include <vector>
#include <cmath>
#include <tuple>


// Prototipos de funciones
std::vector<double> grados_a_radianes(const std::vector<int>& grados);
void CalcularGrados(int Altitud, int Asimuth, int Roll ,int& GAltitude, int& GAsimut, int& GRoll );
std::vector<int> CalcularArreglo1(int GAltitude);
std::vector<int> CalcularArreglo2(int GAsimut);
std::vector<int> CalcularArreglo3(int GRoll);
std::vector<std::vector<int>>CalcularPuntosMovimiento(const std::vector<int>& array1, const std::vector<int>& array2, int radio);
std::vector<std::vector<int>> CalcularPuntosCinematicaInversa(const std::vector<std::vector<int>>& result, int si);
std::tuple<double, double, double> sph2cart(double azimuth, double elevation, double radius);

//Funcion Main
int main() {
    int GAltitude,GAsimut,GRoll;/// Son los datos que regresa la funcion calcularGrados
    int radioEsfera=299;////Esta dato sera ingresado desde la interface de ususario 
    CalcularGrados(6,6,3,GAltitude,GAsimut,GRoll);
    //Muestra los resultados de los grados por posicion
    std::cout << "Grados en Altitude: " << GAltitude << std::endl;
    std::cout << "Grados en Asimuth: " << GAsimut << std::endl; 
    std::cout << "Grados en Roll:" << GRoll << std::endl;



    //Muestra el arreglo de distribuccion de angulos de Azimuth
    std::vector<int> resultado1 = CalcularArreglo1(GAsimut);
    // Imprimir el contenido del vector devuelto
    std::cout << "Los datos del arreglo 1:"<< std::endl;
    for (int value1 : resultado1) {
        std::cout << value1 << " ";
    }
    std::cout<< std::endl;

    //Muestra el arreglo de distribuccion de angulos de Altitude
    std::vector<int> resultado2 = CalcularArreglo2(GAltitude);
    // Imprimir el contenido del vector devuelto
    std::cout << "Los datos del arreglo 2:"<< std::endl;
    for (int value2 : resultado2) {
        std::cout << value2 << " ";
    }
    std::cout<< std::endl;

    //Muestra el arreglo de distribuccion de grados de Roll
    std::vector<int> resultado3 = CalcularArreglo3(GRoll);
    // Imprimir el contenido del vector devuelto
    std::cout << "Los datos del arreglo 3:"<< std::endl;
    for (int value3 : resultado3) {
        std::cout << value3 << " ";
    }
    std::cout<< std::endl;

    // Esta funcion calcula los puntos de posicionamiento de la circunferencia
    //CalcularPuntosMovimiento(resultado1,resultado2,radioEsfera);
    //result continen los datos q1 q2 q3
    std::vector<std::vector<int>> result = CalcularPuntosMovimiento(resultado1,resultado2,radioEsfera);
    //Imprime los datos del vector result
    //for (const std::vector<int>& row : result) {
        //for (int element : row) {
            //std::cout << element << " ";
        //}
        //std::cout << std::endl;
    //} 
   

    // Imprimir el resultado
    // Calculo del tamaño del tamaño de la forma en se movera el robot 
    int si = (resultado1.size() - 1) * (resultado2.size() - 1);
    std::cout<<si<< std::endl;

    ///Imprimir los datos de vector D por separado
    CalcularPuntosCinematicaInversa(result,si); 
   
    return 0;
}


//Cuerpo de las funciones////////////////////////////////////////////////////////////////////////////

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




