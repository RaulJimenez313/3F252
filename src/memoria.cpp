#include <iostream>
#include <list>
using namespace std;

class Persona{
public:
    char nombre[10];
    int edad;
};

class Empleado : public Persona{
public:
    int noEmpleado;
};

class Alumno : public Persona{
public:
    int Registro;
};

int main ( int argc, char const *argv[])
{
    cout << "Tamaño de los tipos de datos: "  << endl;
    cout << "Tamaño int: "  << sizeof(int) << endl;
    cout << "Tamaño long: "  << sizeof(long) << endl;
    cout << "Tamaño float: "  << sizeof(float) << endl;
    cout << "Tamaño double: "  << sizeof(double) << endl;
    cout << "Tamaño bool: "  << sizeof(bool) << endl;
    cout << "Tamaño char: "  << sizeof(char) << endl;
    cout << "Tamaño byte: "  << sizeof(byte) << endl;

    cout << "Direciones de memoria" << endl;
    cout << "Punteros en C: " << malloc(sizeof(int)) << endl;
    cout << "Punteros en C++: " << new int << endl;

    cout << "Tamaño de punteros: "  << endl;
    cout << "int* "  << sizeof(int*) << endl;
    cout << "char* "  << sizeof(char*) << endl;

    cout << "Casteo de tipos: "  << endl;
    //Casteo de puteros
    int* puntero = (int*)malloc(sizeof(int));

    // Casteo de variables
    int ejemplo = 65;
    char letra = (char)ejemplo;
    cout << letra << endl;

    //Casteo polimorfismo
    Empleado* persona = new Empleado();
    Alumno* persona2 = new Alumno();

    list<Persona*> personas;
    personas.emplace_back(persona);
    personas.emplace_back(persona2);

    cout << (*persona) .edad << endl;
    cout << (*persona) .nombre << endl;
    cout << (*persona) .noEmpleado << endl;
    cout << persona->edad << endl;
    cout << persona->nombre << endl;
    cout << persona->noEmpleado << endl;

    //Tratar a todos como personas
    for (auto&&p : personas)
    {
        cout << persona->nombre << endl;
        cout << persona->edad << endl;
    }

    return 0;    
}