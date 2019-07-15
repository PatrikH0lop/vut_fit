SET foreign_key_checks = 0;
DROP TABLE IF EXISTS Zeme;
DROP TABLE IF EXISTS Oblast;
DROP TABLE IF EXISTS Caj;
DROP TABLE IF EXISTS Varka;
DROP TABLE IF EXISTS Obsahuje;
DROP TABLE IF EXISTS Objednavka;
DROP TABLE IF EXISTS Osoba;

DROP TABLE IF EXISTS Vyrizuje;
DROP TABLE IF EXISTS Faktura;
DROP TABLE IF EXISTS Odberatel;
DROP TABLE IF EXISTS Zamestnanec;
DROP TABLE IF EXISTS Admin;
SET foreign_key_checks = 1;

CREATE TABLE Vyrizuje (
  id INTEGER NOT NULL,
  adresa VARCHAR(25) NOT NULL,
  sledovaci_cislo INTEGER NOT NULL,
  id_zamestnanec INTEGER NOT NULL,
  id_objednavka INTEGER NOT NULL
) ENGINE=InnoDB;

CREATE TABLE Faktura (
  id INTEGER NOT NULL AUTO_INCREMENT,
  cena INTEGER NOT NULL,
  PRIMARY KEY(id)
) ENGINE=InnoDB;

CREATE TABLE Osoba (
  id INTEGER NOT NULL PRIMARY KEY,
  jmeno VARCHAR(25) NOT NULL,
  adresa VARCHAR(25) NOT NULL,
  datum_narozeni DATE,
  kontakt VARCHAR(25) NOT NULL,
  login VARCHAR(25) NOT NULL,
  heslo VARCHAR(25) NOT NULL
) ENGINE=InnoDB;

CREATE TABLE Zamestnanec (
  id INTEGER NOT NULL,
  pozice VARCHAR(25) NOT NULL,
  pohlavi VARCHAR(25) NOT NULL,
  CONSTRAINT check_pohlavi CHECK (pohlavi IN ('male', 'female')),
  cislo_uctu VARCHAR(25) NOT NULL,
  plat INTEGER NOT NULL,
  id_veduci INTEGER,
  FOREIGN KEY (id) REFERENCES Osoba (id) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE Odberatel (
  id INTEGER NOT NULL,
  vernostni_body INTEGER,
  datum_registrace DATE,
  FOREIGN KEY (id) REFERENCES Osoba (id) ON DELETE CASCADE
) ENGINE=InnoDB;

CREATE TABLE Oblast
(
  nazev_oblast VARCHAR(25) NOT NULL,
  charakteristika VARCHAR(50),
  popis VARCHAR(100)
) ENGINE=InnoDB;

CREATE TABLE Zeme
(
  nazev_zeme VARCHAR(25) NOT NULL,
  kontinent VARCHAR(20) NOT NULL
) ENGINE=InnoDB;

CREATE TABLE Caj
(
  id INTEGER NOT NULL AUTO_INCREMENT,
  nazev VARCHAR(25) NOT NULL,
  druh VARCHAR(20),
  dodavatel VARCHAR(45) NOT NULL,
  doba_louhovani INTEGER,
  kvalita VARCHAR(10),
  zeme_puvodu VARCHAR(25) NOT NULL,
  nazev_oblasti VARCHAR(25),
  PRIMARY KEY(id)
) ENGINE=InnoDB;

CREATE TABLE Varka
(
  id INTEGER NOT NULL AUTO_INCREMENT,
  cena_100g INTEGER NOT NULL,
  sleva INTEGER NOT NULL,
  mnozstvi INTEGER NOT NULL,
  id_caj INTEGER NOT NULL,
  PRIMARY KEY(id)
) ENGINE=InnoDB;

CREATE TABLE Objednavka
(
  id INTEGER NOT NULL AUTO_INCREMENT,
  stav VARCHAR(20) NOT NULL,
  datum_vytvoreni DATE NOT NULL,
  id_faktura INTEGER,
  id_odberatel INTEGER NOT NULL,
  PRIMARY KEY(id)
) ENGINE=InnoDB;

CREATE TABLE Obsahuje
(
  id INTEGER NOT NULL AUTO_INCREMENT,
  mnozstvi INTEGER NOT NULL,
  cena INTEGER NOT NULL,
  id_objednavka INTEGER NOT NULL,
  id_varka INTEGER NOT NULL,
  PRIMARY KEY(id)
) ENGINE=InnoDB;

CREATE TABLE Admin
(
  id INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY,
  id_zamestnanec INTEGER NOT NULL
) ENGINE=InnoDB;

-- Primary keys
ALTER TABLE Odberatel ADD CONSTRAINT Odberatel_Pkey PRIMARY KEY (id);
ALTER TABLE Zamestnanec ADD CONSTRAINT Zamestnanec_Pkey PRIMARY KEY (id);
ALTER TABLE Oblast ADD CONSTRAINT Oblast_Pkey PRIMARY KEY (nazev_oblast);
ALTER TABLE Zeme ADD CONSTRAINT Zeme_Pkey PRIMARY KEY (nazev_zeme);
ALTER TABLE Vyrizuje ADD CONSTRAINT  Vyrizuje_id PRIMARY KEY (id);

-- Define foreign keys
ALTER TABLE Varka ADD FOREIGN KEY(id_caj) REFERENCES Caj (id);
ALTER TABLE Caj ADD FOREIGN KEY(zeme_puvodu) REFERENCES Zeme (nazev_zeme);
ALTER TABLE Caj ADD FOREIGN KEY(nazev_oblasti) REFERENCES Oblast (nazev_oblast);
ALTER TABLE Obsahuje ADD FOREIGN KEY(id_varka) REFERENCES Varka (id);
ALTER TABLE Obsahuje ADD FOREIGN KEY(id_objednavka) REFERENCES Objednavka (id);
ALTER TABLE Vyrizuje ADD FOREIGN KEY(id_zamestnanec) REFERENCES Zamestnanec (id);
ALTER TABLE Vyrizuje ADD FOREIGN KEY(id_objednavka) REFERENCES Objednavka (id);
ALTER TABLE Objednavka ADD FOREIGN KEY (id_faktura) REFERENCES Faktura (id);
ALTER TABLE Objednavka ADD FOREIGN KEY (id_odberatel) REFERENCES Odberatel (id);
ALTER TABLE Zamestnanec ADD FOREIGN KEY (id_veduci) REFERENCES Zamestnanec (id);
ALTER TABLE Admin ADD FOREIGN KEY (id_zamestnanec) REFERENCES Zamestnanec (id);


-- Add values into tables
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('Shizuoka', 'sunny, warm', 'rocky land, high PH soil');
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('Miyazaki', 'wet, warm', 'very high yealds every year');
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('DongTing', 'wet, clean air', 'rocky, steep hills');

INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Japan', 'Asia');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Cejlon', 'Asia');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Maroco', 'Africa');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Norway', 'Europe');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('China', 'Asia');

INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Sencha Shizu', 'Green', 'Shizu 1732', 3, 'TGFOP', 'Japan', 'Shizuoka');
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Ceylon High Grown', 'Black', 'Manutea', 5, 'OP', 'Cejlon', NULL);
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Gyokuro Yabukita', 'Green', 'Yabukita', 2, 'FOP', 'Japan', 'Miyazaki');
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Gyokuro Yamaguchi', 'Green', 'Yabukita', 3, 'GFOP', 'Japan', 'Miyazaki');
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Tuareg', 'Herb', 'Maroco tea', 6, NULL, 'Maroco', NULL);
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Tuareg extra', 'Herb', 'Maroco tea', 5, NULL, 'Maroco', NULL);
INSERT INTO Caj (nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES('Pi Lo Chun', 'Green', 'Tteeaa', 2, 'TGFOP', 'China', 'DongTing');

INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(3, 140, 0, 1);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(10, 150, 0, 1);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(4, 110, 10, 2);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(10, 90, 0, 3);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(2, 300, 10, 3);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(8, 75, 0, 4);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(12, 60, 10, 5);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(10, 85, 10, 6);
INSERT INTO Varka (mnozstvi, cena_100g, sleva, id_caj) VALUES(9, 135, 0, 7);


INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (1, 'Muhamad Ali', 'Cejl 2, Brno', date '2010-02-15', '0918526175', 'muhamad', '0178268a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat) VALUES (1, 'skladnik', 'male', '0080/000152', 20000);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (2, 'Fero Malachosky', 'Nova 4, Bratislava', date '1992-01-20', '0918501975', 'ferino', '0z78c68a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (2, 'prodejce', 'male', '0080/250152', 28000, 1);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (3, 'Petr Janak', 'Govskeho 8, Praha', date '1990-02-28', '+42015862375', 'malach', '07556xycy');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat) VALUES (3, 'vykonny reditel', 'male', '4080/080152', 12000);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (4, 'Johanna Ostrovskaja', 'Davacnego 2, Moskva', date '1989-01-01', '+18562325412', 'xjohann01', '1926234a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (4, 'recepcni', 'female', '800050/526152', 15000, 1);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (5, 'Jana Modra', 'Davacnego 2, Moskva', date '1979-11-11', '+18560375492', 'xmodra01', '73hgd7354');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (5, 'skladnik', 'female', '7961114447', 11000, 4);

INSERT INTO Admin (id, id_zamestnanec) VALUES (1, 1);

INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (6, 'Microsoft', '5th NewStreet, New York', null, '+4185262375', 'mcrsft', '189162345');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (6, 0, date '1960-10-10');
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (7, 'Redhat', 'Technologicky Park, Brno', null, '91158282375', 'rhos123', '123456789');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (7, 189, date '1980-11-12');
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (8, 'John Clinton', 'Gajdosova 1, Brno', date '1976-05-02', '42018926523', 'clinton1', '08656ds0');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (8, 4230, date '1989-10-12');

-- Testovaci zamestnanci a uzivatele pro snadnejsi login systemu
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (9, 'Test user', 'Street 123', date '1900-01-01', '420100200300', 'user', 'user');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (9, 0, date '1900-1-1');
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (10, 'Test admin', 'Street 123', date '1900-01-01', '420100200301', 'admin', 'admin');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (10, 'Test admin', 'male', '222333/1123', 10000, NULL);
INSERT INTO Admin (id, id_zamestnanec) VALUES (2, 10);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (11, 'Test Employee', 'street 123', date '1900-01-01', '420100200302', 'employee', 'employee');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat) VALUES (11, 'tester', 'male', '222333/1124', 20000);


INSERT INTO Faktura (id, cena) VALUES (1, 180000);
INSERT INTO Faktura (id, cena) VALUES (2, 9600);
INSERT INTO Faktura (id, cena) VALUES (3, 810);

INSERT INTO Objednavka (stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES ('done', date '1985-05-02', 1, 6);
INSERT INTO Objednavka (stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES ('pending', date '1987-01-02', 2, 7);
INSERT INTO Objednavka (stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES ('done', date '2016-03-02', 3, 7);

INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (1, 5, 700, 1, 1);
INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (2, 8, 1200, 2, 2);
INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (3, 3, 270, 3, 4);

INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (1, 'Druha 5, Uherske Hradiste', 94956, 1, 1);
INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (2, 'Moja ulica 1, Nitra', 94901, 2, 2);
INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (3, 'Moja ulica 1, Nitra', 94932, 2, 3);
