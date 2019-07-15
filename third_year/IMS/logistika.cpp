#include "logistika.h"


// nastaveni statickych promennych
int JizdaVozu::str_delka_jizdy = 3;
float JizdaVozu::odch_delka_jizdy = 0.2;

int GeneratorJizd::cekani_dovoz_den = 1200;
int GeneratorJizd::cekani_odvoz_den = 1400;
int GeneratorJizd::cekani_dovoz_noc = 2400;
int GeneratorJizd::cekani_odvoz_noc = 1800;

int SpusteniSimulace::uvodni_kapacita = 0;


JizdaVozu::JizdaVozu(unsigned long int id, bool je_dovozni, int typ_vozu) {
	this->id = id;
	this->dovozni_vuz = je_dovozni;
	this->vytvoren = Time;
	this->typ_vozu = typ_vozu;
}


void JizdaVozu::Behavior() {
	std::string typ_jizdy = (this->dovozni_vuz) ? "Dovozni jizda " : "Odvozni jizda ";
	std::cout << typ_jizdy << id << " vytvorena (typ vozu " << this->typ_vozu << ")" << std::endl;

	zaber_typ_vozidla:
			// pokus zabrat konkretni typ vozidla
			if (sklady_vozidel[this->typ_vozu].Full()) {
				// pokud to nebylo mozne, vlozi se do fronty pro dany typ
				std::cout << typ_jizdy << id << " ceka, nedostupni typ vozu" << std::endl;
				fronty_na_vozy[this->typ_vozu].Insert(this);
				Passivate();
				goto zaber_typ_vozidla;
			}

	Enter(sklady_vozidel[this->typ_vozu], 1); // zaber jedno vozidlo
	std::cout << typ_jizdy << id << " zabrala typ vozidla " << typ_vozu << " (volnych: " << sklady_vozidel[this->typ_vozu].Free() << ")" << std::endl;

	if (this->dovozni_vuz == false) {
		// pokud je to odvozni vuz, pokusi ze zabrat nakladni plosinu
		obsazeni_nakladaci_plosiny:

			if (nakladaci_prostor.Full()) {
				// pokud to nebylo mozne, vlozi se do fronty pro nakladaci plosiny
				std::cout << typ_jizdy << id << ": nepodarilo se obsadit nakladaci prostor, ceka" << std::endl;
				fronta_na_nakladaci_plosinu.Insert(this);
				Passivate();
				goto obsazeni_nakladaci_plosiny;
			}

		std::cout << typ_jizdy << id << ": podarilo se obsadit nakladaci prostor" << std::endl;
		Enter(nakladaci_prostor, 1); // obsazeni plosiny, muze vyrazit z parkoviste
		Wait(Normal(40, 3)); // objizdka budovy
		Wait(Normal(25, 3)); // prijezd k ceste do skladu

		Seize(objezd_budovy); // obsazeni objezdu ke skladu
		Wait(Normal(12, 1)); // Jizda ke skladu
		Release(objezd_budovy);

		Wait(Normal(60*3, 20)); // parkovani a pristaveni vozu ve skladu

		if (sklad.Empty()) {
			// pokud nema co odnest, odjede a pokusi se prijet znovu za 20 minut
			std::cout << typ_jizdy << id << " pripravena ve skladu, ale nema co odnest, prijede znovu" << std::endl;
			Wait(Normal(60, 10)); // odjezd ze skladu
			// uvolni nakladaci prostor ale nedava pokyn pro prijezd
			// dalsiho ve fronte, protoze sklad je prazdny
			Leave(nakladaci_prostor, 1);

			Seize(objezd_budovy);
			Wait(Normal(12, 1));
			Release(objezd_budovy);

			Wait(Normal(25, 3)); // objizdka budovy
			Wait(Normal(40, 3)); // prijezd na puvodni parkoviste
			Wait(Normal(20, 3)); // odstaveni na puvodnim parkovisti
			Activate(Time+60*20); // kontrola za 20 minut

			goto obsazeni_nakladaci_plosiny;
		}

		double odvoz_palety = (int) Normal(odvozni_statistiky[typ_vozu][0], odvozni_statistiky[typ_vozu][1]);
		double odvoz_palety_cp = odvoz_palety;

		while (!sklad.Empty() && odvoz_palety > 0) {
			Leave(sklad, 1); // nakladej pokud jsou palety dostupne
			Wait(Normal(15, 1)); // doba nakladani palety
			odvoz_palety--;
		}

		std::cout << typ_jizdy << id << " nalozila " << odvoz_palety_cp-odvoz_palety << " palet z " << odvoz_palety_cp << ", sklad (" << sklad.Used() << ")" << std::endl;

		Wait(Normal(25, 2)); // potvrzeni dokumentu o prevzeti nakladu
		Wait(15); // uzavreni nakladniho prostoru

		Wait(Normal(60, 10)); // vyjezd ze skladu
		Leave(nakladaci_prostor, 1);
		std::cout << typ_jizdy << id << " vyjela ze skladu" << std::endl;

		// aktivace cekajiciho na danou nakladaci plosinu
		if (fronta_na_nakladaci_plosinu.Length() > 0)
			fronta_na_nakladaci_plosinu.GetFirst()->Activate();

		Seize(objezd_budovy); // objezd budovy
		Wait(Normal(12, 1));
		Release(objezd_budovy);

		Wait(Normal(25, 3)); // odjezd
		Seize(odjezdova_cesta); // zabrani odjezdove cesty
		Wait(Normal(15, 1));
		Release(odjezdova_cesta);

		std::cout << typ_jizdy << id << " opousti areal, odvazi zbozi" << std::endl;
		Wait(Normal(60*60*JizdaVozu::str_delka_jizdy, 60*60*JizdaVozu::odch_delka_jizdy));
		std::cout << typ_jizdy << id << " prichazi do arealu s prazdnym autem" << std::endl;

		Seize(odjezdova_cesta); // prijezd na pozemek
		Wait(Normal(15, 1));
		Release(odjezdova_cesta);

		Wait(Normal(60, 10)); // odstaveni vozu
		Leave(sklady_vozidel[this->typ_vozu], 1); // vraceni vozu

		// aktivace cekajiciho na dany typ vozu
		if (fronty_na_vozy[this->typ_vozu].Length() > 0) 
			fronty_na_vozy[this->typ_vozu].GetFirst()->Activate();

		std::cout << typ_jizdy << id << " konci" << std::endl;

	} else if (this->dovozni_vuz == true) {
			
		Wait(Normal(40, 3)); // obchadzka budovy

		Seize(odjezdova_cesta); // obsazeni vyjezdove cesty
		Wait(Normal(15, 1)); // objezd
		Release(odjezdova_cesta);

		std::cout << typ_jizdy << id << " opousti areal, jde po zbozi" << std::endl;
		Wait(Normal(60*60*JizdaVozu::str_delka_jizdy, 60*60*JizdaVozu::odch_delka_jizdy));
		std::cout << typ_jizdy << id << " prichazi do arealu se zbozim" << std::endl;

		Seize(odjezdova_cesta); // prijizdi nalozene auto
		Wait(Normal(15, 1));
		Release(odjezdova_cesta);

		int pokus_parkoviste = 0;

		if (parkoviste.Full()) {
			// pokud se nepodarilo zabrat parkoviste,
			// odjede vuz na vzdalene parkoviste
			pokus_parkoviste++;
			std::cout << typ_jizdy << id << ": nepodarilo se obsadit parkoviste, odjezd na vzdalene" << std::endl;
			Seize(odjezdova_cesta);
			Wait(Normal(15, 1));
			Release(odjezdova_cesta);
			Wait(Normal(5*60, 20)); // Jizda na vzdalene parkoviste
			std::cout << typ_jizdy << id << " prijela na vzdalene parkoviste" << std::endl;
			fronta_pro_parkoviste.Insert(this); // po prijezdu ceka na signal se vratit
			Passivate();

			Enter(parkoviste, 1); // rezervuje misto na parkovisti
			Wait(Normal(5*60, 20)); // prijezd zpatky
			Seize(odjezdova_cesta);
			Wait(Normal(15, 1));
			Release(odjezdova_cesta);

			goto postup_dal;
		}
		
		std::cout << typ_jizdy << id << ": podarilo se obsadit parkoviste" << std::endl;
		Enter(parkoviste, 1);
		
		postup_dal:

			Wait(Normal(60, 10)); // parkovani na malem parkovisti
			Wait(30); // komunikace se skladem pres telefon

			zabrani_vykladacich_plosin:
				if (vykladaci_prostor.Full()) {
					std::cout << typ_jizdy << id << ": nepodarilo se obsadit vykladaci plosinu" << std::endl;
					fronta_na_vykladaci_plosinu.Insert(this);
					Passivate();
					goto zabrani_vykladacich_plosin;
				}
			
			std::cout << typ_jizdy << id << ": podarilo se obsadit vykladaci plosinu, opousti parkoviste" << std::endl;
			Leave(parkoviste, 1);
			Enter(vykladaci_prostor, 1);

			if (fronta_pro_parkoviste.Length() > 0) // aktivace cekajiciho na vzdalenem parkovisti
				fronta_pro_parkoviste.GetFirst()->Activate();

			Wait(Normal(30, 3)); // prijezd k ceste do skladu
			Seize(objezd_budovy); // obsazeni objezdu ke skladu
			Wait(Normal(12, 1)); // Jizda ke skladu
			Release(objezd_budovy);

			Wait(Normal(3*60, 20)); // parkovani a pristaveni vozu ve skladu
			
			double dovoz_palety = (int) Normal(dovozni_statistiky[this->typ_vozu][0], dovozni_statistiky[this->typ_vozu][1]);
			int dovoz_cp = dovoz_palety;

			while (dovoz_palety > 0 && sklad.Used() + 1 <= sklad.Capacity()) {
				Enter(sklad, 1); // naloz do skladu
				Wait(Normal(15, 1)); // doba vykladani palety
				dovoz_palety--;
			}

			std::cout << typ_jizdy << id << " vylozila " << dovoz_cp-dovoz_palety << " z " << dovoz_cp << ", sklad (" << sklad.Used() << ")" << std::endl;

			Wait(Normal(25, 2)); // potvrzeni dokumentu o vylozeni
			Wait(15); // uzavreni nakladniho prostoru

			Wait(Normal(60, 10)); // vyjezd ze skladu
			std::cout << typ_jizdy << id << " vyjela ze skladu" << std::endl;
			Leave(vykladaci_prostor, 1);

			if (fronta_na_vykladaci_plosinu.Length() > 0) // aktivace cekajiciho na dany typ vozu
				fronta_na_vykladaci_plosinu.GetFirst()->Activate();

			Seize(objezd_budovy); // objezd budovy
			Wait(Normal(12, 1));
			Release(objezd_budovy);

			Wait(Normal(60, 10)); // odstaveni vozu
			Leave(sklady_vozidel[this->typ_vozu], 1); // vraceni vozu

			if (fronty_na_vozy[this->typ_vozu].Length() > 0) // aktivace cekajiciho na dany typ vozu
				fronty_na_vozy[this->typ_vozu].GetFirst()->Activate(); 

			std::cout << typ_jizdy << id << " konci" << std::endl;
	}

	// Odjezd ze systemu
	if (this->dovozni_vuz) {
		vykladaci_auto_v_systemu(Time - this->vytvoren);
	} else {
		nakladaci_auto_v_systemu(Time - this->vytvoren);
	}
}


GeneratorJizd::GeneratorJizd(bool je_dovozni) {
	this->dovozni = je_dovozni;
}


void GeneratorJizd::Behavior() {
	double proba = Random();
	int typ_vozu = 0;
	if (proba >= 0 && proba <= 0.2) typ_vozu = 0;
	else if (proba > 0.2 && proba <= 0.45) typ_vozu = 1;
	else if (proba > 0.45 && proba <= 0.7) typ_vozu = 2;
	else if (proba > 0.7 && proba <= 0.9) typ_vozu = 3;
	else if (proba > 0.9 && proba <= 1.0) typ_vozu = 4;

	(new JizdaVozu(++pocet_jizd, dovozni, typ_vozu))->Activate(Time);

	if (dovozni) {
		if (denni_smena)
			Activate(Time + Exponential(GeneratorJizd::cekani_dovoz_den));
		else
			Activate(Time + Exponential(GeneratorJizd::cekani_dovoz_noc));
	} else {
		if (denni_smena)
			Activate(Time + Exponential(GeneratorJizd::cekani_odvoz_den));
		else
			Activate(Time + Exponential(GeneratorJizd::cekani_odvoz_noc));
	}
}


void Smeny::Behavior()	{
		denni_smena = !denni_smena;
		Activate(Time + 43200);
}


void SpusteniSimulace::Behavior() {
	Enter(sklad, SpusteniSimulace::uvodni_kapacita); // uvodni naplneni skladu
	
	(new GeneratorJizd(true))->Activate(); // generator dovoznich jizd
	(new GeneratorJizd(false))->Activate(); // generator odvoznich jizd
	// Simulace zacina v 0:00, denni smena zacina az v 8:00
	(new Smeny)->Activate(Time + 28800);
}


char *parse_arguments(int argc, char **argv) {

	if (argc != 10) {
		std::cerr << "Wrong number of parameters" << std::endl;
		exit(1);
	}
	
	try {
		nakladaci_prostor.SetCapacity(std::stoi(argv[2]));
		vykladaci_prostor.SetCapacity(std::stoi(argv[3]));
		parkoviste.SetCapacity(std::stoi(argv[4]));
		
		sklady_vozidel[0].SetCapacity(std::stoi(argv[5]));
		sklady_vozidel[1].SetCapacity(std::stoi(argv[6]));
		sklady_vozidel[2].SetCapacity(std::stoi(argv[7]));
		sklady_vozidel[3].SetCapacity(std::stoi(argv[8]));
		sklady_vozidel[4].SetCapacity(std::stoi(argv[9]));
		
		SpusteniSimulace::uvodni_kapacita = (int) (Random() * 5000);

	} catch (...) {
		std::cerr << "Error while converting parameters to numbers" << std::endl;
		exit(1);
	}

	return argv[1];
}


int main(int argc, char **argv)
{
	bool debug = true;

	std::string experiment = parse_arguments(argc, argv);
	std::string name = "results/experiment_" + experiment + ".stat";
	SetOutput(name.c_str());

	nakladaci_auto_v_systemu.Clear();
	vykladaci_auto_v_systemu.Clear();

	// 1 rok
	Init(0, 31536000);
	(new SpusteniSimulace)->Activate();

	Run();

	Print("*** Statistiky ***\n\n");
	
	if (debug)
	{
		for (int i = 0; i < 5; i++) {
			Print("Sklad pro vozidlo typu: %d\n", i);
			sklady_vozidel[i].Output();
			Print("Fronta pro vozidlo typu: %d\n", i);
			fronty_na_vozy[i].Output();
			Print("\n\n");
		}

		sklad.Output();
		nakladaci_prostor.Output();
		fronta_na_nakladaci_plosinu.Output();
		vykladaci_prostor.Output();
		fronta_na_vykladaci_plosinu.Output();
		parkoviste.Output();
		fronta_pro_parkoviste.Output();
		cekani_na_kapacitu_skladu.Output();
		objezd_budovy.Output();
		odjezdova_cesta.Output();
		nakladaci_auto_v_systemu.Output();
		vykladaci_auto_v_systemu.Output();
	}
}
