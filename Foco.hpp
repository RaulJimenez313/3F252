typedef bool EstadoFoco;
class Foco{
private:
    EstadoFoco estado;
public:
    Foco(/* args */) {}
    ~Foco() {}
    void Encender(){estado=true;}
    void Apagar(){estado=false;}
    EstadoFoco LeerEstado(){
        return estado;
    }

};