#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <cmath>
#include <random> 
#include "Dibujo.hpp"
#include "GestorDibujos.hpp"

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
const std::vector<std::u32string> g_bomba = {
    U"  ,  ",
    U" <O> ",
    U"  '  "
};
const std::vector<std::u32string> g_explosion_centro = {
    U" \\ / ",
    U"- * -",
    U" / \\ "
};
const std::vector<std::u32string> g_explosion_horizontal = {
    U"     ",
    U"-----",
    U"     "
};
const std::vector<std::u32string> g_explosion_vertical = {
    U"  |  ",
    U"  |  ",
    U"  |  "
};

// --- Constantes para el tamaño de los dibujos ---
const int TILE_WIDTH = 5;
const int TILE_HEIGHT = 3;

// Estructura para manejar la bomba
struct Bomba {
    Dibujo dibujo_bomba;
    int tiempo_hasta_explosion;
    int tiempo_explosion_restante;
    bool activa;
    bool explotada;

    Bomba(int x, int y) :
        dibujo_bomba(x, y, g_bomba, ftxui::Color::Red),
        tiempo_hasta_explosion(30),
        tiempo_explosion_restante(10),
        activa(true),
        explotada(false) {}
};

// --- FUNCIÓN DE AYUDA para COLISIÓN ---
bool CheckCollision(int obj_x, int obj_y, GestorDibujos& gestor) {
    for (const auto& dibujo_muro : gestor.dibujos) {
        if (dibujo_muro.figura[0] == g_muro_solido[0] || dibujo_muro.color == ftxui::Color::Yellow) {
            if (obj_x == dibujo_muro.x && obj_y == dibujo_muro.y) {
                return true; // Hay colisión
            }
        }
    }
    return false; // No hay colisión
}

// --- FUNCIÓN DE AYUDA para buscar muros destructibles ---
bool IsDestructible(int obj_x, int obj_y, GestorDibujos& gestor) {
    for (const auto& dibujo_muro : gestor.dibujos) {
        if (dibujo_muro.color == ftxui::Color::Yellow) {
            if (obj_x == dibujo_muro.x && obj_y == dibujo_muro.y) {
                return true; // Es un muro destructible
            }
        }
    }
    return false; // No es un muro destructible
}


int main()
{
    // Crear la ventana
    auto screen = Screen::Create(
        Dimension::Fixed(80),
        Dimension::Fixed(30));

    // Crear el gestor de dibujos
    GestorDibujos gestor;

    // Bomberman spawnea en la esquina (1,1)
    Dibujo bomberman(TILE_WIDTH, TILE_HEIGHT, g_bomberman, ftxui::Color::BlueLight);

    // 3 Enemigos
    Dibujo enemigo1(13 * TILE_WIDTH, 7 * TILE_HEIGHT, g_enemigo_globo, ftxui::Color::Red);
    int enemigo1_vel_x = TILE_WIDTH;
    
    Dibujo enemigo2(2 * TILE_WIDTH, 7 * TILE_HEIGHT, g_enemigo_globo, ftxui::Color::Magenta);
    int enemigo2_vel_x = TILE_WIDTH; 

    Dibujo enemigo3(13 * TILE_WIDTH, 2 * TILE_HEIGHT, g_enemigo_globo, ftxui::Color::Cyan);
    int enemigo3_vel_y = TILE_HEIGHT; 
    
    // Variable para la Bomba
    Bomba* bomba_activa = nullptr;
    
    // --- Generación del Laberinto Simétrico ---
    const int MAP_WIDTH_TILES = screen.dimx() / TILE_WIDTH;
    const int MAP_HEIGHT_TILES = screen.dimy() / TILE_HEIGHT;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_destructible(0, 1); // 1 en 2

    for (int y_tile = 0; y_tile < MAP_HEIGHT_TILES; ++y_tile) {
        for (int x_tile = 0; x_tile < MAP_WIDTH_TILES; ++x_tile) {
            int current_x = x_tile * TILE_WIDTH;
            int current_y = y_tile * TILE_HEIGHT;

            // 1. BORDES
            if (x_tile == 0 || x_tile == MAP_WIDTH_TILES - 1 ||
                y_tile == 0 || y_tile == MAP_HEIGHT_TILES - 1) {
                gestor.Agregar(Dibujo(current_x, current_y, g_muro_solido, ftxui::Color::GrayDark));
            }
            
            // 2. PASILLO VACÍO
            else if (x_tile == 1 || x_tile == MAP_WIDTH_TILES - 2 ||
                     y_tile == 1 || y_tile == MAP_HEIGHT_TILES - 2) {
                
                // --- Bloque garantizado MÁS LEJOS ---
                if (x_tile == 6 && y_tile == 1) { // (x=6, y=1)
                    gestor.Agregar(Dibujo(current_x, current_y, g_muro_debil, ftxui::Color::Yellow));
                }
            }

            // 3. INTERIORES FIJOS
            else if (x_tile % 2 == 0 && y_tile % 2 == 0) {
                gestor.Agregar(Dibujo(current_x, current_y, g_muro_solido, ftxui::Color::GrayDark));
            }
            // 4. RESTO DE ESPACIOS
            else {
                // Áreas de spawn despejadas
                if ( !(x_tile == 13 && y_tile == 7) && 
                     !(x_tile == 2 && y_tile == 7) &&
                     !(x_tile == 13 && y_tile == 2) ) 
                { 
                    if (distrib_destructible(gen) == 0) {
                        gestor.Agregar(Dibujo(current_x, current_y, g_muro_debil, ftxui::Color::Yellow)); 
                    }
                }
            }
        }
    }
    
    // --- Bucle Principal del Juego ---
    int frames = 0;
    
    // --- Animación de Bomberman ---
    int bomberman_estado = 0; 
  
    while (true) // Bucle infinito
    {
        screen.Clear();
        frames++;

        // --- Lógica de Bomberman ---
        if (frames % 10 == 0) {
            
            if (bomberman_estado == 0 && frames > 10) {
                bomberman_estado = 1;
            }

            // ESTADO 1: Moviéndose a la derecha buscando un muro para bombardear
            if (bomberman_estado == 1) {
                int next_x = bomberman.x + TILE_WIDTH; 
                int next_y = bomberman.y;
                
                if (IsDestructible(next_x, next_y, gestor)) {
                    // ¡Objetivo encontrado!
                    bomberman_estado = 2; // Pasa a poner la bomba
                }
                else if (CheckCollision(next_x, next_y, gestor)) {
                    bomberman_estado = 99; // Fin del script
                }
                else {
                    bomberman.x = next_x; // Avanza un tile
                }
            }

            // ESTADO 2: Poner la bomba
            if (bomberman_estado == 2) {
                if (bomba_activa == nullptr) {
                    bomba_activa = new Bomba(bomberman.x, bomberman.y); // Pone la bomba ANTES
                }
                bomberman_estado = 3; // Pasa a retroceder
            }
            
            // ESTADO 3: Moverse a la izquierda (a un lugar seguro)
            if (bomberman_estado == 3) {
                // --- CORRECCIÓN 1: Retroceder MÁS (2 tiles) ---
                int safe_x = bomberman.x - (TILE_WIDTH * 2); // Retrocede 2 tiles
                int safe_y = bomberman.y;

                // Chequeo simple (sabemos que (1,1) está vacío)
                if (safe_x >= TILE_WIDTH) {
                    bomberman.x = safe_x; // Se mueve a la izquierda
                }
                bomberman_estado = 4; // Pasa al estado de espera
            }
        }


        // Movimiento de los 3 Enemigos
        if (frames % 15 == 0) { 
            // Enemigo 1 (Rojo) - Horizontal
            int next_x1 = enemigo1.x + enemigo1_vel_x;
            if (CheckCollision(next_x1, enemigo1.y, gestor)) {
                enemigo1_vel_x *= -1; 
            } else {
                enemigo1.x = next_x1;
            }

            // Enemigo 2 (Magenta) - Horizontal
            int next_x2 = enemigo2.x + enemigo2_vel_x;
            if (CheckCollision(next_x2, enemigo2.y, gestor)) {
                enemigo2_vel_x *= -1; 
            } else {
                enemigo2.x = next_x2;
            }

            // Enemigo 3 (Cyan) - Vertical
            int next_y3 = enemigo3.y + enemigo3_vel_y;
            if (CheckCollision(enemigo3.x, next_y3, gestor)) {
                enemigo3_vel_y *= -1; // Invierte velocidad Y
            } else {
                enemigo3.y = next_y3; // Actualiza Y
            }
        }


        // --- Lógica de la Bomba ---
        if (bomba_activa != nullptr) {
            if (bomba_activa->activa) {
                bomba_activa->tiempo_hasta_explosion--;
                if (bomba_activa->tiempo_hasta_explosion <= 0) {
                    bomba_activa->activa = false;
                    bomba_activa->explotada = true;

                    // --- CORRECCIÓN 2: Lógica de Destrucción desde LA BOMBA ---
                    int bomb_tile_x = bomba_activa->dibujo_bomba.x / TILE_WIDTH;
                    int bomb_tile_y = bomba_activa->dibujo_bomba.y / TILE_HEIGHT;

                    std::vector<std::pair<int, int>> tiles_afectados = {
                        {bomb_tile_x, bomb_tile_y}, // El tile de la bomba
                        {bomb_tile_x, bomb_tile_y - 1}, // Arriba
                        {bomb_tile_x, bomb_tile_y + 1}, // Abajo
                        {bomb_tile_x - 1, bomb_tile_y}, // Izquierda
                        {bomb_tile_x + 1, bomb_tile_y}  // Derecha
                    };

                    std::vector<Dibujo> dibujos_que_quedan;
                    for (const auto& dibujo : gestor.dibujos) {
                        bool destruido = false;
                        if (dibujo.color == ftxui::Color::Yellow) { 
                            int muro_tile_x = dibujo.x / TILE_WIDTH;
                            int muro_tile_y = dibujo.y / TILE_HEIGHT;
                            for (const auto& afectado_tile : tiles_afectados) {
                                // Si el muro está en un tile afectado, se destruye
                                if (muro_tile_x == afectado_tile.first && muro_tile_y == afectado_tile.second) {
                                    destruido = true;
                                    break;
                                }
                            }
                        }
                        if (!destruido) {
                            dibujos_que_quedan.push_back(dibujo);
                        }
                    }
                    gestor.dibujos = dibujos_que_quedan; 
                }
            } else if (bomba_activa->explotada) {
                bomba_activa->tiempo_explosion_restante--;
                if (bomba_activa->tiempo_explosion_restante <= 0) {
                    delete bomba_activa;
                    bomba_activa = nullptr;
                }
            }
        }

        // --- Dibujado ---
        gestor.DibujarTodos(screen);
        bomberman.Dibujar(screen);
        enemigo1.Dibujar(screen);
        enemigo2.Dibujar(screen);
        enemigo3.Dibujar(screen);

        if (bomba_activa != nullptr) {
            if (bomba_activa->activa) {
                bomba_activa->dibujo_bomba.Dibujar(screen);
            } else if (bomba_activa->explotada) {
                // --- CORRECCIÓN 3: Dibujar explosión en LA BOMBA ---
                int bomb_draw_x = bomba_activa->dibujo_bomba.x;
                int bomb_draw_y = bomba_activa->dibujo_bomba.y;
                
                Dibujo(bomb_draw_x, bomb_draw_y, g_explosion_centro, ftxui::Color::RedLight).Dibujar(screen);
                Dibujo(bomb_draw_x - TILE_WIDTH, bomb_draw_y, g_explosion_horizontal, ftxui::Color::RedLight).Dibujar(screen);
                Dibujo(bomb_draw_x + TILE_WIDTH, bomb_draw_y, g_explosion_horizontal, ftxui::Color::RedLight).Dibujar(screen);
                Dibujo(bomb_draw_x, bomb_draw_y - TILE_HEIGHT, g_explosion_vertical, ftxui::Color::RedLight).Dibujar(screen);
                Dibujo(bomb_draw_x, bomb_draw_y + TILE_HEIGHT, g_explosion_vertical, ftxui::Color::RedLight).Dibujar(screen);
            }
        }

        // --- Renderizado Final ---
        std::cout << screen.ToString();
        std::cout << screen.ResetPosition();
        std::cout << std::flush;

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // 10 FPS
    }
    
    if (bomba_activa != nullptr) {
        delete bomba_activa;
    }

    return 0;
}