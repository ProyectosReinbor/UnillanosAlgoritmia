#include <iostream>
#include <vector>
#include <map>
#include "json.hpp"
#include <fstream>

using JSON = nlohmann::json;
using namespace std;

class Libro {
    public:
        string titulo;
        string autor;
        int yearPublicacion;
        int numeroCopiasDisponibles;

    Libro(
        string _titulo,
        string _autor,
        int _yearPublicacion,
        int _numeroCopiasDisponibles
    ):
        titulo(_titulo),
        autor(_autor),
        yearPublicacion(_yearPublicacion),
        numeroCopiasDisponibles(_numeroCopiasDisponibles)
    {}

    JSON toJSON() {
        JSON libroJSON;
        libroJSON["titulo"] = titulo;
        libroJSON["autor"] = autor;
        libroJSON["yearPublicacion"] = yearPublicacion;
        libroJSON["numeroCopiasDisponibles"] = numeroCopiasDisponibles;
        return libroJSON;
    }
};

class Usuario {
    public:
        string nombre;
        int identificacion;
        bool estadoMembresia;

    Usuario(
        string _nombre,
        int _identificacion,
        bool _estadoMembresia
    ):
        nombre(_nombre),
        identificacion(_identificacion),
        estadoMembresia(_estadoMembresia)
    {}

    JSON toJSON() {
        JSON usuarioJSON;
        usuarioJSON["nombre"] = nombre;
        usuarioJSON["identificacion"] = identificacion;
        usuarioJSON["estadoMembresia"] = estadoMembresia;
        return usuarioJSON;
    }
};

class PrestamoLibro {
    public:
        int usuarioIdentificacion;
        string libroTitulo;
        int fecha;

    PrestamoLibro(
        int _usuarioIdentificacion,
        string _libroTitulo,
        int _fecha
    ):
        usuarioIdentificacion(_usuarioIdentificacion),
        libroTitulo(_libroTitulo),
        fecha(_fecha)
    {}

    JSON toJSON() {
        JSON prestamoJSON;
        prestamoJSON["usuarioIdentificacion"] = usuarioIdentificacion;
        prestamoJSON["libroTitulo"] = libroTitulo;
        prestamoJSON["fecha"] = fecha;
        return prestamoJSON;
    }
};

class Biblioteca {
    public:
        vector<Libro> libros;
        vector<Usuario> usuarios;
        vector<vector<int>> prestamoLibros;

    Biblioteca() {
        libros = {};
        usuarios = {};
        prestamoLibros = {};
    }

    JSON toJSON() {
        JSON bibliotecaJSON;
        for (int i = 0; i < libros.size(); i++) {
            bibliotecaJSON["libros"][i] = libros[i].toJSON();
        }
        for (int i = 0; i < usuarios.size(); i++) {
            bibliotecaJSON["usuarios"][i] = usuarios[i].toJSON();
        }
        for (int i = 0; i < prestamoLibros.size(); i++) {
            int _indiceLibro = prestamoLibros[i][1];
            PrestamoLibro prestamoLibro = PrestamoLibro(
                prestamoLibros[i][0],
                libros[_indiceLibro].titulo,
                prestamoLibros[i][2]
            );
            bibliotecaJSON["prestamoLibros"][i] = prestamoLibro.toJSON();
        }
        return bibliotecaJSON;
    }
};

string nombreArchivoJSON = "biblioteca.JSON";
Biblioteca biblioteca = Biblioteca();


int cinNumero(string mensaje) {
    string entrada;
    int dato;
    while(true) {
        cout << mensaje;
        cin >> entrada;
        try {
            dato = stoi(entrada);
            break;
        } catch (invalid_argument&) {
            cout << "ERROR: Entrada no valida" << endl;
        } catch (out_of_range&) {
            cout << "Entrada fuera de rango" << endl;
        }
    }
    return dato;
}

bool cinBool(string _mensaje, string _true, string _false) {
    string entrada;
    bool dato;
    while(true) {
        cout << _mensaje << " ( " << _true << " / " << _false << " ): ";
        cin >> entrada;

        if(entrada == _true) {
           return true;
        }
        
        if(entrada == _false) {
            return false;
        }

        cerr << "ERROR: ENTRADA INVALIDA" << endl;
    }
}

int indiceLibroConTitulo(string _titulo) {
    for (int iterador = 0; iterador < biblioteca.libros.size(); iterador++) {
        if (biblioteca.libros[iterador].titulo == _titulo) {
            return iterador;
        }
    }
    return -1;
}

int indiceUsuarioConIdentificacion(int _identificacion) {
    for (int iterador = 0; iterador < biblioteca.usuarios.size(); iterador++) {
        if (biblioteca.usuarios[iterador].identificacion == _identificacion) {
            return iterador;
        }
    }
    return -1;
}

bool registrarNuevoLibro() {
    cout << "Datos del libro" << endl;

    string titulo, autor;
    int yearPublicacion, numeroCopiasDisponibles;

    cout << "Titulo: ";
    cin >> titulo;

    int indiceLibro = indiceLibroConTitulo(titulo);
    for(Libro libro : biblioteca.libros) {
        if(libro.titulo == titulo) {
            cout << "ERROR:EL LIBRO " << titulo << " YA EXISTE" << endl;
            return false;
        }
    }

    cout << "Autor: ";
    cin >> autor;

    yearPublicacion = cinNumero("Año de publicación: ");
    numeroCopiasDisponibles = cinNumero("Número de copias disponibles: ");

    biblioteca.libros.push_back(
        Libro(
            titulo,
            autor,
            yearPublicacion,
            numeroCopiasDisponibles
        )
    );
    cout << "Indice: " << indiceLibroConTitulo(titulo) << endl;
    return true;
}

bool registrarNuevoUsuario() {
    cout << "Registrar un nuevo usuario" << endl;
    cout << "Nombre: ";
    string nombre;
    cin >> nombre;
    int identificacion = cinNumero("Identificacion: ");
    bool estadoMembresia = cinBool("Estado membresia: ", "activa", "bloqueada");

    for(Usuario usuario : biblioteca.usuarios) {
        if(usuario.identificacion == identificacion) {
            cout << "ERROR:EL USUARIO YA EXISTE, IDENTIFICACION: " << identificacion << endl;
            return false;
        }
    }

    biblioteca.usuarios.push_back(
        Usuario(
            nombre,
            identificacion,
            estadoMembresia
        )
    );
    cout << "Indice usuario: " << indiceUsuarioConIdentificacion(identificacion) << endl;
    return true;
}

bool registrarPrestamoLibro()
{
    int _usuarioIdentificacion;
    string _libroTitulo;
    int _fecha;

    cout << "Registrar un prestamo libro" << endl;

    cout << "Usuario identificacion: ";
    cin >> _usuarioIdentificacion;

    cout << "Titulo del libro: ";
    cin >> _libroTitulo;

    cout << "Fecha del prestamo: ";
    cin >> _fecha;

    int indiceUsuario = indiceUsuarioConIdentificacion(_usuarioIdentificacion);
    cout << "Indice usuario: " << indiceUsuario << endl;

    int indiceLibro = indiceLibroConTitulo(_libroTitulo);
    if (indiceLibro == -1)
    {
        cout << "ERROR:EL LIBRO " << _libroTitulo << " NO EXISTE" << endl;
        return false;
    }

    if (biblioteca.libros[indiceLibro].numeroCopiasDisponibles <= 0)
    {
        cout << "ERROR:NO HAY COPIAS DISPONIBLE PARA EL LIBRO " << _libroTitulo << endl;
        return false;
    }

    bool prestamoExiste = false;
    for (int iterador = 0; iterador < biblioteca.prestamoLibros.size(); iterador++)
    {
        if (
            biblioteca.prestamoLibros[iterador][0] == indiceLibro &&
            biblioteca.prestamoLibros[iterador][1] == _usuarioIdentificacion)
        {
            prestamoExiste = true;
            break;
        }
    }

    if (prestamoExiste == true)
    {
        cout << "ERROR:ESTE LIBRO YA HA SIDO PRESTADO AL USUARIO";
        return false;
    }

    biblioteca.libros[indiceLibro].numeroCopiasDisponibles--;

    biblioteca.prestamoLibros.push_back({indiceLibro, _usuarioIdentificacion, _fecha});
    return true;
}

bool registrarDevolucionLibro(string _tituloLibro, int _usuarioIdentificacion)
{
    int indiceLibro = indiceLibroConTitulo(_tituloLibro);
    if (indiceLibro == -1)
    {
        cout << "ERROR:EL LIBRO " << _tituloLibro << " NO EXISTE" << endl;
        return false;
    }

    int indicePrestamo = -1;
    for (int iterador = 0; iterador < biblioteca.prestamoLibros.size(); iterador++)
    {
        if (
            biblioteca.prestamoLibros[iterador][0] == indiceLibro &&
            biblioteca.prestamoLibros[iterador][1] == _usuarioIdentificacion)
        {
            indicePrestamo = iterador;
            break;
        }
    }

    if (indicePrestamo == -1)
    {
        cout << "ERROR: EL PRESTAMO NO EXISTE" << endl;
        return false;
    }

    biblioteca.libros[indiceLibro].numeroCopiasDisponibles++;
    return true;
}

bool mostrarNumeroDeCopiasLibroTitulo(string _titulo)
{
    int indiceLibro = indiceLibroConTitulo(_titulo);
    if (indiceLibro == -1)
    {
        cout << "ERROR: EL LIBRO NO EXISTE" << endl;
        return false;
    }

    cout << "Numero de copias disponibles " << biblioteca.libros[indiceLibro].numeroCopiasDisponibles << endl;
    return true;
}

bool mostrarNumeroDeCopiasLibroAutor(string _autor)
{
    int indiceLibro = -1;
    for (int iterador = 0; iterador < biblioteca.libros.size(); iterador++)
    {
        if (biblioteca.libros[iterador].autor == _autor)
        {
            indiceLibro = iterador;
            break;
        }
    }
    if (indiceLibro == -1)
    {
        cout << "ERROR: EL LIBRO NO EXISTE" << endl;
        return false;
    }

    cout << "Numero de copias disponibles " << biblioteca.libros[indiceLibro].numeroCopiasDisponibles << endl;
    return true;
}

bool mostrarLibrosPrestadosUsuario(int _usuarioIdentificacion)
{
    int indiceUsuario = indiceUsuarioConIdentificacion(_usuarioIdentificacion);
    if (indiceUsuario == -1)
    {
        cout << "ERROR:USUARIO NO EXISTE" << endl;
        return false;
    }

    for (int iterador = 0; iterador < biblioteca.prestamoLibros.size(); iterador++)
    {
        if (biblioteca.prestamoLibros[iterador][1] == _usuarioIdentificacion)
        {
            int indiceLibro = biblioteca.prestamoLibros[iterador][0];
            string titulo = biblioteca.libros[indiceLibro].titulo;
            cout << "Libro " << titulo << " fue prestado al usuario" << endl;
            break;
        }
    }

    return true;
}

bool guardarBiblioteca() {
   ofstream archivo(nombreArchivoJSON);
    if (!archivo.is_open()) {
        cerr << "Error al crear el archivo." << endl;
        return 1;
    }
    JSON bibliotecaJSON = biblioteca.toJSON();
    archivo << bibliotecaJSON.dump(4);
    archivo.close();

    cout << "Archivo JSON creado con éxito." << endl;

    return 0;
}

bool cargarJSON() {
    ifstream archivoJSON(nombreArchivoJSON);
    JSON JSONObjeto;
    if(!archivoJSON.is_open()) {
        cout << "ERROR: No se pudo abrir el archivo JSON" << endl;
        return false;
    }
    try {
        JSONObjeto = JSON::parse(archivoJSON);
    } catch (JSON::parse_error&) {
        cout << "ERROR: No se pudo convertir el archivo JSON" << endl;
        return false;
    }

    for (auto libro : JSONObjeto["libros"]) {
        string titulo = libro["titulo"];
        string autor = libro["autor"];
        int yearPublicacion = libro["yearPublicacion"];
        int numeroCopiasDisponibles = libro["numeroCopiasDisponibles"];
        biblioteca.libros.push_back(
            Libro(
                titulo,
                autor,
                yearPublicacion,
                numeroCopiasDisponibles
            )
        );
    }

    for (auto usuario : JSONObjeto["usuarios"]) {
        string nombre = usuario["nombre"];
        int identificacion = usuario["identificacion"];
        bool estadoMembresia = usuario["estadoMembresia"];
        biblioteca.usuarios.push_back(
            Usuario(
                nombre,
                identificacion,
                estadoMembresia
            )
        );
    }

    for (auto prestamo : JSONObjeto["prestamoLibros"]) {
        int usuarioIdentificacion = prestamo["usuarioIdentificacion"];
        string libroTitulo = prestamo["libroTitulo"];
        int indiceLibro = indiceLibroConTitulo(libroTitulo);
        int fecha = prestamo["fecha"];
        biblioteca.prestamoLibros.push_back({usuarioIdentificacion, indiceLibro, fecha});
    }
    return true;
}

int main() {
    cout << "1111 Bienvenido al sistema de gestión de libros y usuarios." << endl;
    if(cargarJSON() == false) {};

    registrarNuevoUsuario();
    registrarNuevoLibro();
    registrarPrestamoLibro();

    guardarBiblioteca();
    return 0;
}
