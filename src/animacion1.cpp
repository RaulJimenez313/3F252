#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <Dibujo.hpp>
#include <GestorDibujos.hpp>

using namespace ftxui;

const std::vector<std::u32string> g_bomberman = {
    U"[0_0] ",
    U" /|\\ ",
    U" / \\ "
};
const std::vector<std::u32string> g_muro_solido = {
    U"#####",
    U"#####",
    U"#####"
};
const std::vector<std::u32string> g_muro_debil = {
    U"+-+-+",
    U"|-+-|",
    U"+-+-+"
};
const std::vector<std::u32string> g_enemigo_globo = {
    U" / \\ ",
    U"(o.o)",
    U" \\_/ "
};


int main()
{
    auto screen = Screen::Create(
        Dimension::Fixed(80),
        Dimension::Fixed(30));

    GestorDibujos gestor;

    // --- Construir el Nivel ---
    // Muros solidos en los bordes
    for (int x = 0; x < 80; x += 5) {
        gestor.Agregar(Dibujo(x, 0, g_muro_solido, ftxui::Color::GrayDark)); // Borde superior
        gestor.Agregar(Dibujo(x, 27, g_muro_solido, ftxui::Color::GrayDark)); // Borde inferior
    }
    for (int y = 3; y < 27; y += 3) {
        gestor.Agregar(Dibujo(0, y, g_muro_solido, ftxui::Color::GrayDark)); // Borde izquierdo
        gestor.Agregar(Dibujo(75, y, g_muro_solido, ftxui::Color::GrayDark)); // Borde derecho
    }

    // Muros destructibles
    gestor.Agregar(Dibujo(10, 10, g_muro_debil, ftxui::Color::Yellow));
    gestor.Agregar(Dibujo(10, 13, g_muro_debil, ftxui::Color::Yellow));
    gestor.Agregar(Dibujo(10, 16, g_muro_debil, ftxui::Color::Yellow));
    gestor.Agregar(Dibujo(50, 15, g_muro_debil, ftxui::Color::Yellow));
    gestor.Agregar(Dibujo(55, 15, g_muro_debil, ftxui::Color::Yellow));
    
    // Jugador
    gestor.Agregar(Dibujo(8, 5, g_bomberman, ftxui::Color::White));
    int jugador_idx = gestor.dibujos.size() - 1; // Obtenemos el último índice

    // Enemigo
    gestor.Agregar(Dibujo(60, 20, g_enemigo_globo, ftxui::Color::Red));
    int enemigo_idx = gestor.dibujos.size() - 1; // Obtenemos el último índice

    // --- Animacion ---
    int frames = 200;
    int direccion_jugador = 1;
    int direccion_enemigo = -1;

    for (int frame = 0; frame < frames; ++frame)
    {
        screen.Clear();

        // Movimiento jugador 
        gestor.dibujos[jugador_idx].x += direccion_jugador;
        // Rebotar en las paredes
        if (gestor.dibujos[jugador_idx].x > 65 || gestor.dibujos[jugador_idx].x < 8) {
            direccion_jugador *= -1; // Invertir dirección
        }

        // Movimiento enemigo
        gestor.dibujos[enemigo_idx].y += direccion_enemigo;
        // Rebotar
        if (gestor.dibujos[enemigo_idx].y > 22 || gestor.dibujos[enemigo_idx].y < 5) {
            direccion_enemigo *= -1;
        }

        // --- Dibujado ---
        gestor.DibujarTodos(screen);

        // Imprime la pantalla
        std::cout << screen.ToString();
        std::cout << screen.ResetPosition();
        std::cout << std::flush;

        // Pausa
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    return 0;
}