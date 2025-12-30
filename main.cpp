#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <conio.h>

using namespace std;

// Configuración del juego
const int WIDTH = 50;
const int HEIGHT = 20;
const int PIPE_DISTANCE = 20;

struct Bird {
    float y;
    float velocity;
    float gravity;
};

struct Pipe {
    int x;
    int gapY;
    int gapSize;
};

void draw(const Bird& bird, const vector<Pipe>& pipes, int score) {
    // Reposicionar el cursor al inicio para evitar parpadeo (ANSI escape code)
    cout << "\033[H";

    string output = "";
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            // Dibujar el pájaro
            if (x == 10 && y == (int)bird.y) {
                output += "O";
            } 
            // Dibujar tuberías
            else {
                bool isPipe = false;
                for (const auto& pipe : pipes) {
                    if (x >= pipe.x && x <= pipe.x + 2) {
                        if (y < pipe.gapY || y >= pipe.gapY + pipe.gapSize) {
                            output += "#";
                            isPipe = true;
                            break;
                        }
                    }
                }
                
                if (!isPipe) {
                    // Bordes superior e inferior
                    if (y == 0 || y == HEIGHT - 1) {
                        output += "-";
                    } else {
                        output += " ";
                    }
                }
            }
        }
        output += "\n";
    }
    output += "Puntuacion: " + to_string(score) + "    (Espacio para saltar)\n";
    cout << output;
}

int main() {
    // Inicialización
    Bird bird = {HEIGHT / 2.0f, 0.0f, 0.5f};
    vector<Pipe> pipes;
    int score = 0;
    int frame = 0;
    bool gameOver = false;

    // Limpiar pantalla inicialmente
    cout << "\033[2J";

    while (!gameOver) {
        // 1. Entrada (Input)
        if (_kbhit()) {
            char key = _getch();
            if (key == ' ') {
                bird.velocity = -1.5f; // Salto
            }
        }

        // 2. Lógica (Update)
        bird.velocity += bird.gravity;
        bird.y += bird.velocity;

        // Generar tuberías
        if (frame % PIPE_DISTANCE == 0) {
            int gapY = rand() % (HEIGHT - 10) + 3;
            pipes.push_back({WIDTH - 1, gapY, 6});
        }

        // Mover tuberías
        for (int i = 0; i < pipes.size(); ++i) {
            pipes[i].x--;
            
            // Sumar puntuación si el pájaro pasa la tubería
            if (pipes[i].x == 9) {
                score++;
            }
        }

        // Eliminar tuberías fuera de pantalla
        if (!pipes.empty() && pipes[0].x < -3) {
            pipes.erase(pipes.begin());
        }

        // 3. Colisiones
        if (bird.y <= 0 || bird.y >= HEIGHT - 1) {
            gameOver = true;
        }

        for (const auto& pipe : pipes) {
            if (10 >= pipe.x && 10 <= pipe.x + 2) {
                if (bird.y < pipe.gapY || bird.y >= pipe.gapY + pipe.gapSize) {
                    gameOver = true;
                }
            }
        }

        // 4. Dibujar (Render)
        draw(bird, pipes, score);

        // Control de velocidad del juego
        this_thread::sleep_for(chrono::milliseconds(50));
        frame++;
    }

    cout << "\n--- GAME OVER ---" << endl;
    cout << "Puntuacion final: " << score << endl;
    cout << "Presiona cualquier tecla para salir..." << endl;
    _getch();

    return 0;
}
