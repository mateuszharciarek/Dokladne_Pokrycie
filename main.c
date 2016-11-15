/**
 * Zadanie zaliczeniowe nr 3 na zajecia laboratoryjne ze Wstepu do Programowania
 * Uniwersytet Warszawski wydzial Matematyki Informatyki Mechaniki semestr 2015/2016
 *
 * gcc -ansi -pedantic -Wall -Wextra -Werror zadanie_zaliczeniowe_3.c -o zadanie_zaliczeniowe_3
 *
 * autor Mateusz Harciarek <mh359520@students.mimuw.edu.pl>
 * data: 12 stycznia 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*struktura listy mowi nam jaki jest kolejny znak w danym wierszu i na ktorej pozycji w wierszu sie znajduje [bez spacji] (pn. "a c" c jest na 3 miejscu)*/
struct pole
{
    char znak;
    int pozycja;
    struct pole *nast;
};

/*struktura listy dwukierunkowej gdzie trzymamy pierwsze pola kazdego wiersza z wejscia*/
struct wiersz
{
    struct pole *p;
    struct wiersz *nast;
    struct wiersz *poprz;
};

/*tworzy nowe pole i przypisuje mu wartosci z argumentow*/
struct pole *utworz_pole(int poz,char z,struct pole *nastepny)
{
    struct pole *wynik = malloc(sizeof(struct pole));
    wynik -> pozycja = poz;
    wynik -> znak = z;
    wynik -> nast = nastepny;
    return wynik;
}

/*wsortowuje w liste wierszy nastepny zaczynajacy sie argumentem "pol"*/
struct wiersz *utworz_wiersz(struct pole *pol,struct wiersz *w)
{
    struct wiersz *wynik = malloc(sizeof(struct wiersz));

    /*gdy jest to nasz pierwszy wiersz*/
    if (w == NULL)
    {
        wynik -> p = pol;
        wynik -> nast = w;
        wynik -> poprz = NULL;
    }
    else
    {
        wynik -> p = pol;
        w -> poprz = wynik;
        wynik -> nast = w;
        wynik -> poprz = NULL;
    }

    return wynik;
}

/*usuwa liste pol od przekazanego w argunemcie*/
void usuwanie_pola(struct pole *l)
{
    struct pole *pom = l;
    while(l != NULL)
    {
        pom = l -> nast;
        free(l);
        l = pom;
    }
}

/*usuwa wiersz czyli liste pol gdzie w -> p jest pierwsze i sama struktore wiersz*/
void usuwanie_wiersza(struct wiersz *w)/*??? usuwa dwa razy w -> p ??? */
{
    struct pole *pom_p = w -> p;

    usuwanie_pola(pom_p);
    free(w);
}

/*odwraca liste pol*/
struct pole *odwracanie_pola(struct pole *l)
{
    struct pole *odw = NULL,*tmp = NULL;

    while(l != NULL)
    {
        tmp = l -> nast;
        l -> nast = odw;
        odw = l;
        l = tmp;
    }
    return odw;
}

/*odwraca liste wierszy*/
struct wiersz *odwracanie_wiersza(struct wiersz *w)
{
    struct wiersz *tmp = NULL,*odw = NULL;

    while(w != NULL)
    {
        tmp = w -> nast;
        w -> nast = w -> poprz;
        w -> poprz = tmp;

        odw = w;
        w = tmp;
    }

    return odw;
}

/**wchodzi p1 == NULL
 *wczytuje wiersz z wejscia i zamienia go na liste pol
 *jako argument mamy int max_w, dzieki ktormu mozemy po wszystkich wierszach odczytac maksymalna dlugosc wiersza
 */
struct pole *wiersz_na_liste(struct pole *p1,int *max_w)
{
    int i = 1;
    char z;

    /*dopuki nie bedzie blednych danych lub znaku == 10 co oznacza nowa linie i koniec tego wiersza*/
    while(scanf("%c",&z) == 1 && z != 10)
    {
        /*wstawiamy do listy znaki ignorujac spacje*/
        if(z != ' ')
        {
            p1 = utworz_pole(i,z,p1);
            *max_w = i;
        }
        i++;
    }
    /*nowe pola dodawalismy zawsze na poczatek listy wiec aby uzyskac pola od "najwczesniejszych" odwracamy liste*/
    p1 = odwracanie_pola(p1);
    return p1;
}

/**wchodzi w1 == NULL i max = 0
 *tworzymy liste wierszy
 * argument max bedzie po wykonaniu funkcji rowny maksymalnej dlugosci wiersza
 */
struct wiersz *lista_wierszy(struct wiersz *w1,int *max)
{
    char a = 0;
    int p_max = 0;/*podejzenie max*/
    struct pole *pom = NULL;

    /*zamiast scanf uzywamy funkcji getchar aby sprawdzac pierwszy znak w wierszu i w odpowiednim momencie zakonczyc funkcje*/
    a = getchar();
    ungetc(a,stdin);

    while(a != EOF)
    {
        /*tworzymy liste calego wiersza*/
        pom = wiersz_na_liste(pom,&p_max);
        /*umieszczamy wiersz w liscie wierszy*/
        w1 = utworz_wiersz(pom,w1);
        pom = NULL;

        /*wyznaczanie max*/
        if(p_max > *max)
            *max = p_max;

        a = getchar();
        ungetc(a,stdin);
    }

    /*w tej postaci programu ta funkcjia chyba nie jest konieczna? dzieki niej otrzymyjemy liste wierszy od pierwszego do ostatniego*/
    w1 = odwracanie_wiersza(w1);

    return w1;
}

/*sortuje liste wierszy tak, ze pierwszenstwo maja wiersze ktore maja pierwszy znak na wczesniejszej pozycji*/
struct wiersz *porzadek_wedlug_pozycji(struct wiersz *w1)
{
    /**
     *sort - znacznik przed ktorym juz posortowano wiersze
     * ob - element obecnie sortowany
     * tmp - element ktory docelowo ma byc pierwszym mniejszym od ob
     */
    struct wiersz *sort = w1,*ob = w1 -> nast,*tmp;

    while(sort -> nast != NULL)
    {
        /*przechodzimy przez fragmenty posortowane*/
        while((ob -> nast != NULL) && (ob -> p -> pozycja >= sort -> p -> pozycja))
        {
            sort = ob;
            ob = ob -> nast;
        }

        /*gdy ob jest najmniejszy*/
        if(ob -> p -> pozycja <= w1 -> p -> pozycja)
        {
            /*jesli ob jest ostatnim elementem*/
            if(ob -> nast == NULL)
            {
                sort -> nast = NULL;
            }
            else
            {
                sort -> nast = ob -> nast;
                ob -> nast -> poprz = sort;
            }
            ob -> nast = w1;
            ob -> poprz = NULL;
            w1 -> poprz = ob;
            w1 = ob;
            ob = sort -> nast;
        }
            /*ob nie idzie na poczatek*/
        else
        {
            tmp = sort;
            /*nie wejdziemy na poczatek listy bo sprawdzalismy to wczesniej*/
            while(tmp -> p -> pozycja > ob -> p ->pozycja)
                tmp = tmp -> poprz;

            if(ob -> nast != NULL)
            {
                if(tmp != sort)
                {
                    sort -> nast = ob -> nast;
                    ob -> nast -> poprz = sort;
                    ob -> nast = tmp -> nast;
                    tmp -> nast -> poprz = ob;
                    tmp -> nast = ob;
                    ob -> poprz = tmp;
                    ob = sort -> nast;
                }
                else
                {
                    sort = ob;
                    ob = ob -> nast;
                }
            }
            else
            {
                if(tmp != sort)
                {
                    sort -> nast = NULL;
                    tmp -> nast -> poprz = ob;
                    ob -> nast = tmp -> nast;
                    tmp -> nast = ob;
                    ob -> poprz = tmp;
                }
                else
                {
                    sort = ob;
                }
            }
        }
    }

    return w1;
}

/**
 * w1 przed w2 wynik 1
 * w2 przed w1 wynik -1
 * w1 rowne w2 wynik 0
 * zamiast wartosci -1,0,1 mozna bylo stworzyc typ okrojony ale w obecnej postaci nie jest to az tak kosztowna operacja aby ja zmieniac
 * porownuje ktory wierszjest "mniejszy" czyli powinien byc w liscie przed drugim porownywanym
 * sprawdzenie wedlug pozycji i alfabetu czyli otrzymujemy np:
 *
 * a
 * b
 * c
 *  a
 *  ab
 *  abcd
 *  abce
 */
int porownaj_wiersze(struct wiersz *w1,struct wiersz *w2)
{
    struct pole *p_w1 = w1 -> p,*p_w2 = w2 -> p;
    /*petla dziala dopoki znaki sa te same i sa na tych samych pozycjach*/
    while(p_w1 != NULL && p_w2 != NULL && p_w1 -> pozycja == p_w2 -> pozycja && p_w1 -> znak == p_w2 -> znak)
    {
        p_w1 = p_w1 -> nast;
        p_w2 = p_w2 -> nast;
    }

    /*oba wiersze zakonczyly sie a ich poprzednie pola sa identycne*/
    if(p_w1 == NULL && p_w2 == NULL)
        return 0;

    /*sprawdzamy NULL jeszcze raz. Gdy ktorys warunek jest spelniony od razu wychodzimy z funkcji jesli nie sprawdzamy nastepny wiec nie trzeba robic rozbudowanego "if-elsa"*/
    if(p_w1 == NULL)
        return 1;
    if(p_w2 == NULL)
        return -1;

    /*w tym miejscu wiemy ze p_w1,p_w2 != NULL sprawdzamy najpierw pozycje aby wczesniej byly wyrazy o 'wczesniejszych polach wypelnionych'*/
    if(p_w1 -> pozycja < p_w2 -> pozycja)
        return 1;
    else
    if(p_w1 -> pozycja > p_w2 -> pozycja)
        return -1;

    /*tu dochodzimy tylko jesli p_w1 -> pozycja == p_w2 -> pozycja i wtedy dopiero sprawdzamy kolejnosc znakow*/
    if(p_w1 -> znak < p_w2 -> znak)
        return 1;
    else
        return -1;

}

/*wchodzi lista wierszy posortowana wedlug pozycji
 *funkcja sortuje wiersze wedlug alfabetu (nie psujac sortowania wedlug pozycji)
 */
struct wiersz *porzadek_wedlug_alfabetu(struct wiersz *w1)
{
    /**
     * pocz - wyznacza poczatek sortowanego fragmentu o tej samej pozycji pierwszego znaku
     * ob - obecnie sortowany element
     * tmp - zmienna pomocnicza
     * nast_ob - wyznacza element ktory w nastepnym obrocie petli bedzie ob
     */
    struct wiersz *pocz = w1,*ob = w1 -> nast,*tmp,*nast_ob;
    int ustawienie;

    /*ob = NULL oznacza ze ostatni "blok" ma tylko jeden element (leniwe wyliczania)*/
    while(ob != NULL && ob -> nast != NULL)
    {
        /*sortujemy wiersze ktore maja pierwszy znak w tym samym miejscu*/
        while(ob != NULL && pocz -> p -> pozycja == ob -> p -> pozycja)
        {
            nast_ob = ob -> nast;

            /*ob na poczatek*/
            if(ob -> p -> znak <= pocz -> p -> znak)
            {
                /*pierwszy sortowany "blok" wierszy*/
                if(pocz -> poprz == NULL)
                {
                    ob -> poprz -> nast = ob -> nast;
                    if (ob -> nast != NULL)
                        ob -> nast -> poprz = ob -> poprz;
                    ob -> nast = pocz;
                    ob -> poprz = NULL;
                    pocz -> poprz = ob;
                    pocz = ob;
                    w1 = pocz;
                }
                else
                {
                    ob -> poprz -> nast = ob -> nast;
                    if (ob -> nast != NULL)
                        ob -> nast -> poprz = ob -> poprz;
                    ob -> nast = pocz;
                    ob -> poprz = pocz -> poprz;
                    pocz -> poprz -> nast = ob;
                    pocz = ob;
                }
            }
            else
            {
                tmp = ob;
                /*nigdy nie wyjdziemy przed pocz*/
                do
                {tmp = tmp -> poprz;}
                while(ob -> p -> znak < tmp -> p -> znak);

                /*aby wejsc do petli sprawdzamy wrunek przed petla a nastepnie raz dla tych samych danycy*/
                ustawienie = porownaj_wiersze(ob,tmp);
                while(ustawienie == 1)
                {
                    ustawienie = porownaj_wiersze(ob,tmp);
                    /*warunek zapobiega niepotrzebnej zmianie tmp gdy wychodzimy z petli*/
                    if(ustawienie == 1)
                        tmp = tmp -> poprz;
                }
                /*wiersze sa takie same i jeden usuwamy*/
                if(ustawienie == 0)
                {
                    /*ob jest ostatnim sortowanym elementem*/
                    if(ob -> nast == NULL)
                    {
                        ob -> poprz -> nast = NULL;
                        usuwanie_wiersza(ob);
                    }
                    else
                    {
                        ob -> poprz -> nast = ob -> nast;
                        ob -> nast -> poprz = ob -> poprz;
                        usuwanie_wiersza(ob);
                    }
                }
                    /*wiersze sa rozne i znamy poprawne tmp (poprzedzajace umieszczenie ob)*/
                else
                {
                    /*jesli ob ma zostac jako ostatni to nic nie robimy*/
                    if (tmp != ob -> poprz)
                    {
                        ob -> poprz -> nast = ob -> nast;
                        if (ob -> nast != NULL)
                            ob -> nast -> poprz = ob -> poprz;
                        ob -> poprz = tmp;
                        tmp -> nast -> poprz = ob;
                        ob -> nast = tmp -> nast;
                        tmp -> nast = ob;
                    }
                }
            }
            ob = nast_ob;
        }
        pocz = ob;
        if(ob != NULL)
            ob = ob -> nast;
    }


    /*w czesci funkcji powyzej nie usuwalismy tych samych wierszy, ktore sa "pierwszym posortowanym wierszem" wiec tobimy to teraz*/
    pocz = w1 -> nast;

    ustawienie = porownaj_wiersze(pocz -> poprz,pocz);
    while(ustawienie == 0 && pocz -> nast != NULL)
    {
        usuwanie_wiersza(pocz -> poprz);
        pocz = pocz -> nast;
        ustawienie = porownaj_wiersze(pocz -> poprz,pocz);
        if (ustawienie != 0)
        {
            pocz = pocz -> poprz;
            pocz -> poprz = NULL;
            w1 = pocz;
        }
    }

    return w1;
}

/*---------------------------------------*/
/**tu zaczynaja sie funkcje zwiazane z glowna czescia programu
 *wszystko powyzej bylo po to aby posortowac wiersze i znalezc te, ktore sie powtarzaja i je usunac aby nie zwracac kilku takich samych rozwiazan
 */

/**w tablicy czy_wypelnione mamy informacje czy ta pozycja w wierszu jest wolna
 *funkcja sprawdza czy dany wiersz mozna wpisac do naszego rozwiazania
 */
_Bool czy_wiersz_pasuje_do_tablicy(struct wiersz *w,_Bool czy_wypelnione[])
{
    struct pole *p = w -> p;
    _Bool czy_ok = true;
    while(p != NULL && czy_ok == true)
    {
        if(czy_wypelnione[p -> pozycja] == true)
            czy_ok = false;
        else
            p = p -> nast;
    }

    return czy_ok;
}

/**uzupelnia tablice o znaki z danego wiersza
 * UWAGA !!!
 * funkcja nie sprawdza poprawnosci danych (moze nadpisywac przy zlych danych)
 */
void dopisz_wiersz(struct wiersz *w,_Bool *czy_wypelnione,char *tablica)
{
    struct pole *p = w -> p;
    while(p != NULL)
    {
        tablica[p -> pozycja] = p -> znak;
        czy_wypelnione[p -> pozycja] = true;
        p = p -> nast;
    }
}

/*usuwa z rozwiazania wiersz w, nie musimy znieniac tablicy "tablica" gdyz przy nastepnym uzupelnianiu ja po prostu nadpiszemy*/
void wymaz_wiersz(struct wiersz *w,_Bool *czy_wypelnione)
{
    struct pole *p = w -> p;
    while(p != NULL)
    {
        czy_wypelnione[p -> pozycja] = false;
        p = p -> nast;
    }
}

/*informuje czy mamy caly wypelniony wiersz*/
_Bool czy_jest_caly_wiersz(int dl_wiersza,_Bool czy_wypelnione[])
{
    int i = 1;
    _Bool b = true;
    while(b == true && i <= dl_wiersza)
    {
        if(czy_wypelnione[i] == false)
            b = false;
        i++;
    }

    return b;
}

/*funkcja wypisuje pojedyncze rozwiazanie zadania*/
void wypisz_caly_wiersz(char tablica[],int dl_wiersza)
{
    int i;
    for(i = 1;i <= dl_wiersza;i++)
        printf("%c",tablica[i]);
    /**nie jestem pewien czy po ostatnim wypisanym wierszu nie przejdziemy niepotrzebnie do nowej lini
     * wedlug mnie tak powinno sie stac co jest bledem,
     * jednak gdy testowalem program w pliku wyjsciowym wszystko jest tak jak powinno
     */
    printf("\n");
}

/**glowna funkcja programu sprawdzajaca rekurencyjnie rozwiazania
 *rozwiazania trzymamy w tablicach:
 * czy_wypelnione - mowi czy na danej pozycji jest juz wpisany jakis znak
 * tablica - przechowuje znaki do rozwiazania
 */
void rekurencja(struct wiersz *w,char tablica[],_Bool czy_wypelnione[],int dl_wiersza)
{
    _Bool czy_w_pasuje,czy_caly_wiersz;

    if(w != NULL)
    {
        /*sprawdzamy czy obecny wiersz mozna dopisac do tablic*/
        czy_w_pasuje = czy_wiersz_pasuje_do_tablicy(w,czy_wypelnione);

        /*gdy wiersz pasuje*/
        if(czy_w_pasuje)
        {
            /*dopisujemy wiersz do rozwiazania*/
            dopisz_wiersz(w,czy_wypelnione,tablica);
            /*sprawdzamy czy mamy cale rozwiazanie*/
            czy_caly_wiersz = czy_jest_caly_wiersz(dl_wiersza,czy_wypelnione);
            if(czy_caly_wiersz)
                wypisz_caly_wiersz(tablica,dl_wiersza);
            /*wywolujemy rekurencje z tablicami uzupelnionymi o nowy wiersz*/
            rekurencja(w -> nast,tablica,czy_wypelnione,dl_wiersza);
            /*usuwamy dopisany wiersz aby kontynuowac sprawdzanie rozwiazan od nastepnego wiersza ale bez obecnego(obecny sprawdzalismy w poprzednim wywolaniu)*/
            wymaz_wiersz(w,czy_wypelnione);
        }
        rekurencja(w -> nast,tablica,czy_wypelnione,dl_wiersza);
    }
}

/*funkcjia zwalnia pamiec calej struktury wierszy*/
void zwolnij_pamiec(struct wiersz *w)
{
    struct wiersz *usun = w;
    w = w -> nast;
    while(w != NULL)
    {
        usuwanie_wiersza(usun);
        usun = w;
        w = w -> nast;
    }
    /*po wyjsciu z petli zostal jeszcze jeden element*/
    usuwanie_wiersza(usun);
}

int main()
{
    struct wiersz *w1 = NULL;
    int dl_wiersza = 0,i;

    char *tablica;
    _Bool *czy_wypelnione;

    w1 = lista_wierszy(w1,&dl_wiersza);

    /*+1 bo tablice zaczynamy od 0
     * pole 0 jest puste ale tracimy na tym tylko sizeof(int)+sizeof(_Bool),
     * ale za to wszystkie operacje na tablicach sa czytelne i bez przesuniec o 1
     */

    /*alokujemy tablice o naszym rozmiarze +1 (jak komentarz wyzej)*/
    tablica = (char *) malloc((dl_wiersza+1)*sizeof(char));
    czy_wypelnione = (_Bool *) malloc((dl_wiersza+1)*sizeof(_Bool));

    /*sortujemy wiersze*/
    w1 = porzadek_wedlug_pozycji(w1);
    w1 = porzadek_wedlug_alfabetu(w1);

    for(i = 0;i < dl_wiersza;i++)
    {
        czy_wypelnione[i] = false;
        /*wypelnienie dla prawidlowego programu nie powinno miec znaczenia*/
        tablica[i] = 'a';
    }


    rekurencja(w1,tablica,czy_wypelnione,dl_wiersza);

    /*zwalniamy cala zaalokowana pamiec*/
    free(tablica);
    free(czy_wypelnione);
    zwolnij_pamiec(w1);

    return 0;
}