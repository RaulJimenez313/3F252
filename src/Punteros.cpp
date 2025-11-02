#include <iostream>
#include <string>
#include <list>
#include <vector> // Es buena práctica usar vector si no necesitas inserciones/borrados en medio

using namespace std;

// Clase Base
class Humano {
private:
    string nombre;
    int edad;
public:
    // Constructor de la clase Humano
    Humano(string nombre, int edad) {
        this->nombre = nombre;
        this->edad = edad;
    }

    // Definición de los métodos "getters"
    string LeerNombre() {
        return this->nombre;
    }
    int LeerEdad() {
        return this->edad;
    }
};

// Clase Derivada
class Alumno : public Humano {
private: // Es mejor encapsular los datos
    int registro;
public:
    // Constructor de Alumno, que llama al constructor de Humano
    Alumno(string nombre, int edad, int registro) : Humano(nombre, edad) {
        this->registro = registro;
    }

    // Definición del método para leer el registro
    int LeerRegistro() { // Corregido: Esto debe ser un método/función
        return this->registro;
    }
};

// Clase Derivada
class Empleado : public Humano {
private: // Encapsulando el dato
    int noEmpleado;
public:
    // Constructor de Empleado, que llama al constructor de Humano
    Empleado(string nombre, int edad, int noEmpleado) : Humano(nombre, edad) {
        this->noEmpleado = noEmpleado;
    }

    // Definición del método para leer el número de empleado
    int LeerNumeroEmpleado() { // Corregido: Esto también debe ser un método
        return this->noEmpleado;
    }
};

int main(int argc, char const *argv[]) {
    // Creamos una lista de punteros a Humano
    list<Humano*> listaHumanos;

    // Agregamos objetos de tipo Alumno y Empleado a la lista
    // usando los constructores que definimos
    listaHumanos.emplace_back(new Alumno("Juan Perez", 20, 12345));
    listaHumanos.emplace_back(new Alumno("Maria Lopez", 22, 54321));
    listaHumanos.emplace_back(new Empleado("Ana Gomez", 35, 101));
    listaHumanos.emplace_back(new Empleado("Luis Martinez", 45, 102));

    cout << "--- Lista de Personas ---" << endl;

    // Recorremos la lista e imprimimos los datos usando los métodos
    for (auto const& humano : listaHumanos) {
        cout << "Nombre: " << humano->LeerNombre()
             << ", Edad: " << humano->LeerEdad() << endl;
    }

    // IMPORTANTE: Liberar la memoria que pedimos con "new"
    for (auto& humano : listaHumanos) {
        delete humano;
    }
    listaHumanos.clear(); // Vaciamos la lista

    return 0;
}