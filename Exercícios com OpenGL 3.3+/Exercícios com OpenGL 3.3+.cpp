#include <iostream>

void runParte1();
void runParte2();

int main() {
    int opcao = -1;

    while (opcao != 0) {
        std::cout << "=== Menu ===\n";
        std::cout << "1. Parte 1 - Triangulos fixos\n";
        std::cout << "2. Parte 2 - Triangulos com transformacoes (GLM)\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opcao: ";
        std::cin >> opcao;

        switch (opcao) {
        case 1:
            runParte1();
            break;
        case 2:
            runParte2();
            break;
        case 0:
            std::cout << "Saindo do programa...\n";
            break;
        default:
            std::cout << "Opcao invalida. Tente novamente.\n";
        }
    }

    return 0;
}
