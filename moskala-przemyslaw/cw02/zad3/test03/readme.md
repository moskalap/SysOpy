Napisz program umożliwiający w trybie interaktywnym (tekstowym) wykonanie następujących operacji dla pliku będącego jego argumentem:

    ustawienie rygla do odczytu na wybrany znak pliku,
    ustawienie rygla do zapisu na wybrany znak pliku,
    wyświetlenie listy zaryglowanych znaków pliku (z informacją o numerze PID procesu będącego właścicielem rygla oraz jego typie - odczyt/zapis),
    zwolnienie wybranego rygla,
    odczyt (funkcją read) wybranego znaku pliku,
    zmiana (funkcją write) wybranego znaku pliku.

Operacje 1 oraz 2 powinny być dostępne w dwóch wariantach, różniących się zachowaniem w przypadku, gdy ustawienie rygla jest niemożliwe ze względu na rygle założone przez inne procesy:

    wersja nieblokująca - próba ustawienia rygla kończy się wypisaniem informacji o błędzie
    wersja blokująca - program czeka, aż ustawienie rygla będzie możliwe

Wybór znaku we wszystkich przypadkach polega na podaniu numeru bajtu w pliku. Do ryglowania należy wykorzystać funkcję fcntl i strukturę flock. Wykonaj eksperymenty uruchamiając program jednocześnie w dwóch terminalach dla tego samego pliku. Próbuj ryglować (do odczytu lub do zapisu) te same znaki pliku i modyfikować (lub odczytywać) wybrane znaki pliku wykorzystując zarówno blokujące, jak i nieblokujące wersje operacji. Zwróć uwagę, że domyślnie ryglowanie działa w trybie rygli zalecanych (advisory), a nie rygli wymuszanych (mandatory).