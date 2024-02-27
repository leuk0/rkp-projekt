#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma Makrók/Globális változók 
// Színek
#define FG_COLOR_BLACK   "\x1b[30m"
#define FG_COLOR_RED     "\x1b[31m"
#define FG_COLOR_GREEN   "\x1b[32m"
#define FG_COLOR_YELLOW  "\x1b[33m"
#define FG_COLOR_BLUE    "\x1b[34m"
#define FG_COLOR_MAGENTA "\x1b[35m"
#define FG_COLOR_CYAN    "\x1b[36m"
#define FG_COLOR_WHITE   "\x1b[37m"

// Háttér színek
#define BG_COLOR_BLACK   "\x1b[40m"
#define BG_COLOR_RED     "\x1b[41m"
#define BG_COLOR_GREEN   "\x1b[42m"
#define BG_COLOR_YELLOW  "\x1b[43m"
#define BG_COLOR_BLUE    "\x1b[44m"
#define BG_COLOR_MAGENTA "\x1b[45m"
#define BG_COLOR_CYAN    "\x1b[46m"
#define BG_COLOR_WHITE   "\x1b[47m"

// Szín visszaállítás
#define COLOR_RESET   "\x1b[0m"

// Cimkék
#define TAG_INFO "[INFO] "
#define TAG_DEBUG FG_COLOR_CYAN "[DEBUG]" COLOR_RESET " "
#define TAG_ERROR FG_COLOR_RED "[ERROR]" COLOR_RESET " "
#define TAG_CRITICAL FG_COLOR_WHITE BG_COLOR_RED "[CRITICAL]" COLOR_RESET " "

// Hibakódok
#define EXIT_FAILURE 1 // Ez még nincs használva de később még jól jöhet
#define EXIT_INVALID_ARGUMENT 2
#define EXIT_INVALID_PROGRAM_NAME 3

// Verziószám, dátum, fejlesztő
#define VERSION "v0.1.3"
#define DATE "2024. február 25."
#define AUTHOR "Divinyi Balázs"

// Üzemmódok
#define MODE_SEND "-send"
#define MODE_RECEIVE "-receive"

// Kommunikációs módok
#define COMM_FILE "-file"
#define COMM_SOCKET "-socket"

// Kapcsolók
#define FLAG_HELP "--help"
#define FLAG_VERSION "--version"

// Beállított módok // Azért kell globális vátozó, mert 'process_arguments()'-ben és a 'main()'-ben is használjuk ezeket
char *mode = MODE_SEND;
char *comm = COMM_FILE;
#pragma endregion

// Segítség kiíratása
void print_help()
{
    printf("Használat:\n");
    printf("  chart [kapcsoló(k)]\n");
    printf("\nOpciók:\n");
    printf("  --version      Verziószám kiírása\n");
    printf("  --help        Súgó kiírása\n");
    printf("\nÜzemmódok:\n");
    printf("  -send          Adatok küldése (alapértelmezett)\n");
    printf("  -receive       Adatok fogadása\n");
    printf("\nKommunikációs módok:\n");
    printf("  -file          Fájlok használata (alapértelmezett)\n");
    printf("  -socket        Socket használata\n");
}

// Ugyan az mint a 'print_help()' csak itt le is áll a program
void print_help_then_exit()
{
    print_help();
    exit(EXIT_SUCCESS);
}

// Program név ellenőrzése
void check_program_name(char *argv[])
{
    // Azért van szükség a './'-re, mert az kell a futtatáshoz és az is beleszámít a program nevébe
    if (strcmp(argv[0], "./chart") != 0)
    {
        fprintf(stderr, TAG_ERROR "A program neve nem 'chart'! (%s)\n", argv[0]);
        exit(EXIT_INVALID_PROGRAM_NAME);
    }
}

// Parancssori argumentumok feldolgozása
int process_arguments(int argc, char *argv[])
{
    // Itt létrehozzuk az 'i' változót amit lentebb a for ciklusokban használunk, így csak egyszer kell deklarálni
    int i;

    // Verzió, dátum és fejlesztő kiíratása, verzió kapcsoló ellenörzése
    if (argc > 1 && strcmp(argv[1], FLAG_VERSION) == 0)
    {
        printf("Verzió: %s\n", VERSION);
        printf("Dátum: %s\n", DATE);
        printf("Fejlesztő: %s\n", AUTHOR);
        exit(EXIT_SUCCESS);
    }

    // Segítség kapcsoló ellenörzése
    if (argc > 1 && strcmp(argv[1], FLAG_HELP) == 0)
    {
        print_help_then_exit();
        // Ide nem kell 'return EXIT_SUCCESS' mivel a program leáll a 'print_help_then_exit()' metóduson belül
    }

    // Itt megnézzük hogy az argumentumok között szerepel-e a '-send' vagy '-receive' kapcsoló
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], MODE_SEND) == 0 || strcmp(argv[i], MODE_RECEIVE) == 0)
        {
            mode = argv[i];
            break;
        }
    }

    // Itt megnézzük hogy az argumentumok között szerepel-e a '-file' vagy '-socket' kapcsoló
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], COMM_FILE) == 0 || strcmp(argv[i], COMM_SOCKET) == 0)
        {
            comm = argv[i];
            break;
        }
    }

    // Itt megnézzük hogy az argumentumok között szerepel-e bármelyik előre definiált kapcsoló és ha nem,
    // akkor egy hibaüzenet után kiíratjuk a segítséget majd visszatérünk a 'main()'-be az ahol a program leáll
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], MODE_SEND) != 0 && strcmp(argv[i], MODE_RECEIVE) != 0 && strcmp(argv[i], COMM_FILE) != 0 && strcmp(argv[i], COMM_SOCKET) != 0 && strcmp(argv[i], FLAG_HELP) && strcmp(argv[i], FLAG_VERSION))
        {
            fprintf(stderr, TAG_ERROR "Érvénytelen argumentum(ok)!\n\n");
            print_help();
            return EXIT_INVALID_ARGUMENT;
        }
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    // Program név ellenőrzése
    check_program_name(argv);

    // Parancssori argumentumok feldolgozása
    int ret = process_arguments(argc, argv);
    if (ret != EXIT_SUCCESS)
    {
        return ret;
    }

    // Módok tesztelése
    if (strcmp(mode, MODE_SEND) == 0)
    {
        printf(TAG_DEBUG "Küldő üzemmód\n");
    }
    else if (strcmp(mode, MODE_RECEIVE) == 0)
    {
        printf(TAG_DEBUG "Fogadó üzemmód\n");
    }

    // Kommunikácisó módok tesztelése
    if (strcmp(comm, COMM_FILE) == 0)
    {
        printf(TAG_DEBUG "Fájlal történő kommunikáció\n");
    }
    else if (strcmp(comm, COMM_SOCKET) == 0)
    {
        printf(TAG_DEBUG "Socketel történő kommunikáció\n");
    }

    return EXIT_SUCCESS;
}
