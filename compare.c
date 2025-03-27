#include <stdio.h>
#include <stdlib.h>

int compare_files(FILE *file1, FILE *file2) {
    char ch1, ch2;
    // Porównuj pliki znak po znaku
    while ((ch1 = fgetc(file1)) != EOF && (ch2 = fgetc(file2)) != EOF) {
        if (ch1 != ch2) {
            return 0;  // Pliki są różne
        }
    }
    // Sprawdzenie, czy oba pliki zakończyły się w tym samym momencie
    if (fgetc(file1) != EOF || fgetc(file2) != EOF) {
        return 0;  // Pliki mają różne długości
    }
    return 1;  // Pliki są identyczne
}

int main(int argc, char *argv[]) {
    // Sprawdzanie, czy zostały podane dwa argumenty (nazwy plików)
    if (argc != 3) {
        fprintf(stderr, "Błąd: Program wymaga dwóch argumentów (nazw plików).\n");
        return 1;
    }

    FILE *file1, *file2;

    // Otwarcie pierwszego pliku
    file1 = fopen(argv[1], "r");
    if (file1 == NULL) {
        perror("Błąd otwierania pierwszego pliku");
        return 1;
    }

    // Otwarcie drugiego pliku
    file2 = fopen(argv[2], "r");
    if (file2 == NULL) {
        perror("Błąd otwierania drugiego pliku");
        fclose(file1);  // Zamknij wcześniej otwarty plik
        return 1;
    }

    // Porównanie plików
    if (compare_files(file1, file2)) {
        printf("Pliki są identyczne.\n");
    } else {
        printf("Pliki są różne.\n");
    }

    // Zamknięcie plików
    fclose(file1);
    fclose(file2);

    return 0;
}
