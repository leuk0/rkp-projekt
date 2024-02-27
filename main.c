#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#pragma Makrók / Globális változók
// Színek
#define FG_COLOR_BLACK "\x1b[30m"
#define FG_COLOR_RED "\x1b[31m"
#define FG_COLOR_GREEN "\x1b[32m"
#define FG_COLOR_YELLOW "\x1b[33m"
#define FG_COLOR_BLUE "\x1b[34m"
#define FG_COLOR_MAGENTA "\x1b[35m"
#define FG_COLOR_CYAN "\x1b[36m"
#define FG_COLOR_WHITE "\x1b[37m"

// Háttér színek
#define BG_COLOR_BLACK "\x1b[40m"
#define BG_COLOR_RED "\x1b[41m"
#define BG_COLOR_GREEN "\x1b[42m"
#define BG_COLOR_YELLOW "\x1b[43m"
#define BG_COLOR_BLUE "\x1b[44m"
#define BG_COLOR_MAGENTA "\x1b[45m"
#define BG_COLOR_CYAN "\x1b[46m"
#define BG_COLOR_WHITE "\x1b[47m"

// Szín visszaállítás
#define COLOR_RESET "\x1b[0m"

// Cimkék
#define TAG_INFO "[INFO] "
#define TAG_DEBUG FG_COLOR_CYAN "[DEBUG]" COLOR_RESET " "
#define TAG_ERROR FG_COLOR_RED "[ERROR]" COLOR_RESET " "
#define TAG_CRITICAL FG_COLOR_WHITE BG_COLOR_RED "[CRITICAL]" COLOR_RESET " "

// Hibakódok
#define EXIT_FAILURE 1
#define EXIT_INVALID_ARGUMENT 2
#define EXIT_INVALID_PROGRAM_NAME 3

// Verziószám, dátum, fejlesztő
// v[ha kész 1].[készen lévő feladatig].[javítások]
#define VERSION "v0.2.1"
#define DATE "2024. február 27."
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
#define FLAG_DEBUG "--debug"

// Measurement
#define MEASUREMENT_TIME 900        // 900s = 15m / reset ideje másodpercbe
#define MEASUREMENT_MIN_TIME 100    // Hány másodperc múlva induljon tovább a generálás
#define MEASUREMENT_MIN_COUNT 100   // Hány számot generáljon

// True | False
#define TRUE 1
#define FALSE 0

// Beállított módok // Azért kell globális vátozó hogy a 'process_arguments()'-ben módosítani tudjuk az értékét és a
char *mode = MODE_RECEIVE;
char *comm = COMM_FILE;

// Debug
int debug = FALSE;
int debug_comms_modes = TRUE;
int debug_measurements = TRUE;
#pragma endregion

// Segítség kiíratása
void print_help()
{
    printf("Használat:\n");
    printf("  chart [kapcsoló(k)]\n");
    printf("\nOpciók:\n");
    printf("  --version      Verziószám kiírása\n");
    printf("  --help        Súgó kiírása\n");
    printf("  --debug        Debug mode\n");
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

    // Itt megnézzük hogy az argumentumok között szerepel-e a '--debug' kapcsoló
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], FLAG_DEBUG) == 0)
        {
            debug = TRUE;
        }
    }

    // Itt megnézzük hogy az argumentumok között szerepel-e bármelyik előre definiált kapcsoló és ha nem,
    // akkor egy hibaüzenet után kiíratjuk a segítséget majd visszatérünk a 'main()'-be az ahol a program leáll
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], MODE_SEND) != 0 && strcmp(argv[i], MODE_RECEIVE) != 0 && strcmp(argv[i], COMM_FILE) != 0 && strcmp(argv[i], COMM_SOCKET) != 0 && strcmp(argv[i], FLAG_HELP) && strcmp(argv[i], FLAG_VERSION) && strcmp(argv[i], FLAG_DEBUG))
        {
            fprintf(stderr, TAG_ERROR "Érvénytelen argumentum(ok)!\n\n");
            print_help();
            return EXIT_INVALID_ARGUMENT;
        }        
    }

    return EXIT_SUCCESS;
}

// Függvény, amely létrehozza és feltölti a megfelelő méretű tömböt a kért adatokkal
int Measurement(int **Values)
{
    // Itt megnézzük/megkapjuk hogy az utolsó negyedóra (xx:00, xx:15, xx:30, xx:45) óta eltelt másodperceket
    time_t calendar_object = time(NULL);
    struct tm *tm_struct = localtime(&calendar_object);
    int seconds_from_quarter = tm_struct->tm_hour * 3600 + tm_struct->tm_min * 60 + tm_struct->tm_sec;

    // Inicializálás
    srand(time(NULL));

    // Megfelelő méretű tömb létrehozása és inicializálása 1/2
    int measurement = (seconds_from_quarter % MEASUREMENT_TIME) + 1;

    // Measurement debug
    if (debug && debug_measurements)
    {
        printf(TAG_DEBUG "Variable: seconds_from_quarter: %d\n", seconds_from_quarter);
        printf(TAG_DEBUG "Variable: measurement: %d\n", measurement);
    }

    // Megfelelő méretű tömb létrehozása és inicializálása 2/2
    *Values = (int *)malloc(measurement * sizeof(int));
    if (*Values == NULL)
    {
        fprintf(stderr, TAG_CRITICAL "Memóriafoglalási hiba!\n");
        exit(EXIT_FAILURE);
    }

    // Az első érték beállítása
    int value = 0;
    (*Values)[0] = value;

    // 100 értéket generál addig amíg az eltelt másodpercek száma kevesebb mint 100
    if (measurement < MEASUREMENT_MIN_TIME) { measurement = MEASUREMENT_MIN_COUNT; }

    // Értékek generálása a kért szabályoknak megfelelően
    for (int i = 1; i < measurement; i++)
    {
        int random_number = rand() % 31 + 1; // 1-31 közötti véletlen szám generálása

        // Az előző érték függvényében új érték generálása
        if (random_number <= 11) { value--; }
        else if (random_number >= 21) { value++; }
        (*Values)[i] = value;
    }

    // Visszatérés a generált értékek számával
    return measurement;
}

int main(int argc, char *argv[])
{
    // Program név ellenőrzése
    check_program_name(argv);

    // Parancssori argumentumok feldolgozása
    int ret = process_arguments(argc, argv);
    if (ret != EXIT_SUCCESS) { return ret; }

    // Módok tesztelése
    if (debug && debug_comms_modes)
    {
        if (strcmp(mode, MODE_SEND) == 0) { printf(TAG_DEBUG "Küldő üzemmód\n"); }
        else if (strcmp(mode, MODE_RECEIVE) == 0) { printf(TAG_DEBUG "Fogadó üzemmód\n"); }

        // Kommunikácisó módok tesztelése
        if (strcmp(comm, COMM_FILE) == 0) { printf(TAG_DEBUG "Fájlal történő kommunikáció\n"); }
        else if (strcmp(comm, COMM_SOCKET) == 0) { printf(TAG_DEBUG "Socketel történő kommunikáció\n"); }
    }
    int measurement_counter = 0;

    // Mérési értékek tárolására szolgáló tömb létrehozása és feltöltése
    int *measurement_values;
    int measurement = Measurement(&measurement_values);
    
    if (debug && debug_measurements) { printf(TAG_DEBUG "Mért értékek:\n"); }

    for (int i = 0; i < measurement; i++)
    {
        if (debug && debug_measurements) { printf("%d ", measurement_values[i]); }
        measurement_counter++;
    }

    if (debug && debug_measurements) { printf("\n" TAG_DEBUG "Mért értékek száma: %d\n", measurement_counter); }

    free(measurement_values);
    return EXIT_SUCCESS;
}
