Zadanie 2. Operacje na strukturze katalogów

Napisz program przyjmujący dwa argumenty wywołania: ścieżkę do katalogu w systemie plików, oraz liczbę całkowitą oznaczającą rozmiar pliku w bajtach. Po uruchomieniu program przeszukuje drzewo katalogów zakorzenione w ścieżce przekazanej w pierwszym argumencie i szuka w nim plików zwyczajnych o rozmiarze nie większym niż wielkość podana jako drugi argument. Dla każdego znalezionego pliku wypisywane są następujące informacje:

    ścieżka bezwzględna pliku
    rozmiar w bajtach
    prawa dostępu do pliku (w formacie używanym przez ls -l, np. r--r--rw- oznacza prawa odczytu dla wszystkich i zapisu dla właściciela)
    datę ostatniej modyfikacji

Ścieżka podana jako argument wywołania może być względna lub bezwzględna. Program powinien wyszukiwać tylko pliki zwyczajne (bez dowiązań, urządzeń blokowych, itp). Program nie powinien podążać za dowiązaniami symbolicznymi do katalogów.

Program należy zaimplementować w dwóch wariantach:

    Korzystając z funkcji opendir, readdir oraz funkcji z rodziny stat
    Korzystając z funkcji nftw
