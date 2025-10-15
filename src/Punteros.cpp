#include <iostream>
#include <string>
#include <list>
using namespace std;

class Humano{
    private:
        string nombre;
        int edad;
    public:
        string LeerNombre();
        int LeerEdad();
};

class Alumno : public Humano{
    public:
       int registro;
    public:
        int Leerregistro;
};

class Empleado : public Humano{
    public:
       int noEmpleado;
    public:
        int LeerNumeroEmpleado;
};
int main ( int argc, char const *argv[])
{
    Humano* humano = NULL;
    Alumno* alumno = new Alumno;
    Empleado* empleado = new Empleado;

    humano = alumno;
    humano->LeerEdad();

    humano = empleado;
    humano->LeerEdad();
    
    list <Humano*> listaHumanos;
    listaHumanos.emplace_back(new Alumno());
    listaHumanos.emplace_back(new Alumno());
    listaHumanos.emplace_back(new Alumno());
    listaHumanos.emplace_back(new Alumno());
    listaHumanos.emplace_back(new Alumno());
    listaHumanos.emplace_back(new Empleado());
    listaHumanos.emplace_back(new Empleado());
    listaHumanos.emplace_back(new Empleado());
    listaHumanos.emplace_back(new Empleado());
    listaHumanos.emplace_back(new Empleado());

    for (auto &&humano : listaHumanos)
    {
        cout << humano->LeerNombre() << endl;
    }
    
    return 0;
};