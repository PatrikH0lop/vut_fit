#ifndef PARKOVISTE
#define PARKOVISTE

#include <iostream>
#include "simlib.h"


// vozidla - 5 typu, pocty vozu jsou parametr
Store sklady_vozidel[5];
// fronty pro jednotlive typy vozidel
Queue fronty_na_vozy[5];

/* stredni hodnoty a odchylky pro pocty palet privazene a odvazene
   jednotlivymi typy vozidel */
int odvozni_statistiky[5][2] = {
	{6, 1},
	{12, 1},
	{16, 1},
	{31, 2},
	{33, 2}
};

int dovozni_statistiky[5][2] = {
	{7, 2},
	{12, 2},
	{18, 1},
	{32, 2},
	{31, 1}
};

// sklad, kapacita skladu je parametr
Store sklad("sklad", 5000);
Queue cekani_na_kapacitu_skladu("Fronta pro uvolneni skladu");

// Nakladaci a vykladaci plosiny, pocty jsou parametr
Store nakladaci_prostor("Nakladaci prostor", 4);
Queue fronta_na_nakladaci_plosinu("Fronta pro nakladaci plosinu");
Store vykladaci_prostor("Vykladaci prostor", 2);
Queue fronta_na_vykladaci_plosinu("Fronta pro vykladaci plosinu");

// Cesta kolem skladu
Facility objezd_budovy("Objezd budovy");
// Odjezdova cesta
Facility odjezdova_cesta("Odjezdova cesta z arealu");

// Parkoviste, kapacita parkoviste je parametr
Store parkoviste("Parkoviste pro dovozni vozy", 26);
Queue fronta_pro_parkoviste("Fronta pro parkoviste");

// Indikator denni/nocni smeny (zaciname v 0:00)
bool denni_smena = false;

// Sledovane hodnoty
Histogram vykladaci_auto_v_systemu("Celkova doba vykladacich aut v systemu", 0, 1000, 30);
Histogram nakladaci_auto_v_systemu("Celkova doba nakladacich aut v systemu", 0, 1000, 30);

// procesy a udalosti

// Modelovani jizdy vozu
class JizdaVozu : public Process
{
    public:
        static int str_delka_jizdy;
        static float odch_delka_jizdy;

        unsigned long int id; // id vozu
        bool dovozni_vuz; // true pro dovozni jizdu, false pre odvozni
        double vytvoren; // cas zahajeni jezdy
        int typ_vozu; // typ nakladniho vozu

        JizdaVozu(unsigned long int id, bool je_dovozni, int typ_vozu);
        void Behavior();
};

// Generator jizd
class GeneratorJizd : public Event
{
    public:
        static int cekani_dovoz_noc;
        static int cekani_dovoz_den;
        static int cekani_odvoz_noc;
        static int cekani_odvoz_den;
   
        unsigned long int pocet_jizd = 0; // pocet vygenerovanych jizd
        bool dovozni; // true pro dovozni jizdy, false pro odvozni
        
        GeneratorJizd(bool je_dovozni);
        void Behavior();
};

// Prepinac mezi denni a nocni smenou
// Denni smena probiha kazdy den mezi 8. a 20. hod
class Smeny : public Event {
    public:
        void Behavior();
};

// Uvodne naplneni skladu a spusteni generatoru
class SpusteniSimulace: public Process {
    public:
        static int uvodni_kapacita;
        void Behavior();
};

#endif
