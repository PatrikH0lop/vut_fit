----------------------------------------
-- Authors: Matej Hrabal, Patrik Holop
-- Logins: xhraba12, xholop01
----------------------------------------

-- Drop all tables (Will cause warning if tables dont exists)
-- xhraba12 tables
DROP TABLE Oblast CASCADE CONSTRAINTS;
DROP TABLE Zeme CASCADE CONSTRAINTS;
DROP TABLE Caj CASCADE CONSTRAINTS;
DROP TABLE Varka CASCADE CONSTRAINTS;
DROP TABLE Objednavka CASCADE CONSTRAINTS;
DROP TABLE Obsahuje CASCADE CONSTRAINTS;
-- xholop01 tables
DROP TABLE Odberatel CASCADE CONSTRAINTS;
DROP TABLE Osoba CASCADE CONSTRAINTS;
DROP TABLE Zamestnanec CASCADE CONSTRAINTS;
DROP TABLE Vyrizuje CASCADE CONSTRAINTS;
DROP TABLE Faktura CASCADE CONSTRAINTS;

-- Drop sequences
DROP SEQUENCE Caj_ID_Holder;

-- Create tables
CREATE TABLE Vyrizuje (
  id INTEGER NOT NULL,
  adresa VARCHAR2(25) NOT NULL,
  sledovaci_cislo INTEGER NOT NULL,
  id_zamestnanec INTEGER NOT NULL,
  id_objednavka INTEGER NOT NULL
);

CREATE TABLE Faktura (
  id INTEGER NOT NULL,
  cena INTEGER NOT NULL
);

CREATE TABLE Osoba (
  id INTEGER NOT NULL PRIMARY KEY,
  jmeno VARCHAR2(25) NOT NULL,
  adresa VARCHAR2(25) NOT NULL,
  datum_narozeni DATE,
  kontakt VARCHAR2(25) NOT NULL,
  login VARCHAR2(25) NOT NULL,
  heslo VARCHAR2(25) NOT NULL
);

CREATE TABLE Zamestnanec (
   id INTEGER NOT NULL,
  pozice VARCHAR2(25) NOT NULL,
  pohlavi VARCHAR2(25) NOT NULL,
  CONSTRAINT check_pohlavi CHECK (pohlavi IN ('muz', 'zena')),
  cislo_uctu VARCHAR2(25) NOT NULL,
  plat INTEGER NOT NULL,
  id_veduci INTEGER,
  CONSTRAINT GKZ_OSOBA FOREIGN KEY (id) REFERENCES Osoba ON DELETE CASCADE
);

CREATE TABLE Odberatel (
  id INTEGER NOT NULL,
  vernostni_body INTEGER,
  datum_registrace DATE,
  CONSTRAINT GKO_OSOBA FOREIGN KEY (id) REFERENCES Osoba ON DELETE CASCADE
);

CREATE TABLE Oblast
(
  nazev_oblast VARCHAR2(25) NOT NULL,
  charakteristika VARCHAR2(50),
  popis VARCHAR2(100)
);

CREATE TABLE Zeme
(
  nazev_zeme VARCHAR2(25) NOT NULL,
  kontinent VARCHAR2(20) NOT NULL
);

CREATE TABLE Caj
(
  id INTEGER NOT NULL,
  nazev VARCHAR2(25) NOT NULL,
  druh VARCHAR2(20),
  dodavatel VARCHAR2(45) NOT NULL,
  doba_louhovani NUMBER NOT NULL,
  kvalita VARCHAR2(10),
  zeme_puvodu VARCHAR2(25) NOT NULL,
  nazev_oblasti VARCHAR2(25)
);

CREATE TABLE Varka
(
  id INTEGER NOT NULL,
  cena_100g NUMBER NOT NULL,
  mnozstvi NUMBER NOT NULL,
  id_caj INTEGER NOT NULL
);

CREATE TABLE Objednavka
(
  id INTEGER NOT NULL,
  stav VARCHAR2(20) NOT NULL,
  datum_vytvoreni DATE NOT NULL,
  id_faktura INTEGER,
  id_odberatel INTEGER NOT NULL
);

CREATE TABLE Obsahuje
(
  id INTEGER NOT NULL,
  mnozstvi NUMBER NOT NULL,
  cena NUMBER NOT NULL,
  id_objednavka INTEGER NOT NULL,
  id_varka INTEGER NOT NULL
);

-- Primary keys
ALTER TABLE Odberatel ADD CONSTRAINT Odberatel_Pkey PRIMARY KEY (id);
ALTER TABLE Zamestnanec ADD CONSTRAINT Zamestnanec_Pkey PRIMARY KEY (id);
ALTER TABLE Faktura ADD CONSTRAINT Faktura_Pkey PRIMARY KEY (id);

ALTER TABLE Oblast ADD CONSTRAINT Oblast_Pkey PRIMARY KEY (nazev_oblast);
ALTER TABLE Zeme ADD CONSTRAINT Zeme_Pkey PRIMARY KEY (nazev_zeme);
ALTER TABLE Caj ADD CONSTRAINT Caj_Pkey PRIMARY KEY (id);
ALTER TABLE Varka ADD CONSTRAINT Varka_Pkey PRIMARY KEY (id);
ALTER TABLE Objednavka ADD CONSTRAINT Objednavka_Pkey PRIMARY KEY (id);
ALTER TABLE Vyrizuje ADD CONSTRAINT  Vyrizuje_id PRIMARY KEY (id);
ALTER TABLE Obsahuje ADD CONSTRAINT  Obsahuje_id PRIMARY KEY (id);

-- Define foreign keys
ALTER TABLE Varka ADD CONSTRAINT Caj_Fkey FOREIGN KEY(id_caj) REFERENCES Caj;
ALTER TABLE Caj ADD CONSTRAINT Zeme_Fkey FOREIGN KEY(zeme_puvodu) REFERENCES Zeme;
ALTER TABLE Caj ADD CONSTRAINT Oblast_Fkey FOREIGN KEY(nazev_oblasti) REFERENCES Oblast;
ALTER TABLE Obsahuje ADD CONSTRAINT Varka_Fkey FOREIGN KEY(id_varka) REFERENCES Varka;
ALTER TABLE Obsahuje ADD CONSTRAINT Objednavka_Fkey FOREIGN KEY(id_objednavka) REFERENCES Objednavka;
ALTER TABLE Vyrizuje ADD CONSTRAINT Zamestnanec_Fkey FOREIGN KEY(id_zamestnanec) REFERENCES Zamestnanec;
ALTER TABLE Vyrizuje ADD CONSTRAINT Objednavka_Fkey2 FOREIGN KEY(id_objednavka) REFERENCES Objednavka;

ALTER TABLE Objednavka ADD CONSTRAINT Faktura_Fkey FOREIGN KEY (id_faktura) REFERENCES Faktura;
ALTER TABLE Objednavka ADD CONSTRAINT VOdberatel_Fkey FOREIGN KEY (id_odberatel) REFERENCES Odberatel;

ALTER TABLE Zamestnanec ADD CONSTRAINT Veduci_Fkey FOREIGN KEY (id_veduci) REFERENCES Zamestnanec;

-- TRIGGERY
-- Trigger pro automaticke prirazeni id caje
CREATE SEQUENCE Caj_ID_holder
  START WITH 1
  INCREMENT BY 1
  CACHE 100;

CREATE OR REPLACE TRIGGER trigger1
BEFORE INSERT ON Caj
FOR EACH ROW
  BEGIN
    :new.id := Caj_ID_Holder.nextval;
  END trigger1;
/

-- Ukazka prvniho triggeru
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Cina', 'Asie');
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(NULL, 'Pai Mu Tan', 'White', 'Fujian export', 1, 'OP', 'Cina', NULL);

-- Trigger, ktery pro telefonni cislo prida predcisli +420, pokud zadne predcisli nema (V tom pripade se predpoklada ze se jedna o ceske cislo)
CREATE OR REPLACE TRIGGER trigger2
AFTER INSERT
ON Osoba
BEGIN
 UPDATE Osoba
 SET kontakt =
  CASE
    WHEN SUBSTR(kontakt, 1, 1) <> '+'
      THEN CONCAT('+420', kontakt)
    ELSE
      kontakt
 END;
END trigger2;
/


-- PROCEDURY
-- Procedura, ktera zvysi plat zamestnance, pokud existuje a vyridil alespon jednu objednavku
-- Plat se zvysuje o hodnotu bonus a urcuje se ID zamestnance, kteremu plat zvysujeme
-- Ukazka funkcnosti je na konci programu (Aby se zavolal INSERT)
SET serveroutput ON;
CREATE OR REPLACE PROCEDURE premie(bonus IN INTEGER, id_zamestnance INTEGER) IS
CURSOR obj_plat IS
  SELECT DISTINCT Z.id, O.jmeno, Z.plat FROM
  Zamestnanec Z LEFT JOIN Osoba O ON Z.id = O.id LEFT JOIN Vyrizuje V ON V.id_zamestnanec = Z.id LEFT JOIN Objednavka Ob on V.id_objednavka = Ob.id
  WHERE Z.id = id_zamestnance AND V.id_objednavka IS NOT NULL;
  odk obj_plat%ROWTYPE;
  PODMINKY EXCEPTION;
  it BOOLEAN;
BEGIN
  it := FALSE;
  OPEN obj_plat;
  LOOP
    FETCH obj_plat INTO odk;
    IF it = TRUE
      THEN EXIT;
    END IF;
    IF obj_plat%NOTFOUND
      THEN RAISE PODMINKY;
    END IF;
    DBMS_OUTPUT.PUT_LINE('Rozdavam premie');
    DBMS_OUTPUT.PUT_LINE('ID: ' || odk.id || ', Jmeno: ' || odk.jmeno || ' Ziskava premii ' || bonus || 'Kc');
    DBMS_OUTPUT.PUT_LINE('Plat pred: ' || odk.plat);
    odk.plat := odk.plat + bonus;
    DBMS_OUTPUT.PUT_LINE('Plat nyni: ' || odk.plat);
    UPDATE Zamestnanec
      SET plat = plat + bonus
      WHERE id = id_zamestnance;
    it := TRUE;
  END LOOP;
  EXCEPTION
  WHEN NO_DATA_FOUND THEN
    DBMS_OUTPUT.PUT_LINE('Osoba s id ' || id_zamestnance || ' neexistuje nebo nesplnuje podminky pro zisk premii');
  WHEN PODMINKY THEN
    DBMS_OUTPUT.PUT_LINE('Osoba s id ' || id_zamestnance || ' neexistuje nebo nesplnuje podminky pro zisk premii');
  WHEN OTHERS THEN
    Raise_Application_Error (-1, 'ERROR');
END;
/


CREATE OR REPLACE PROCEDURE podozrive_kontakty
IS
   CURSOR Zamestnanci IS SELECT * FROM Zamestnanec LEFT JOIN Osoba O ON Zamestnanec.id = O.id;
   odkaz Zamestnanci%ROWTYPE;
BEGIN
  OPEN Zamestnanci;
  LOOP
    FETCH Zamestnanci into odkaz;
    EXIT WHEN Zamestnanci%NOTFOUND;
      IF (odkaz.kontakt NOT LIKE '+____________')
        THEN
          dbms_output.put_line('Zamestnanec ' || odkaz.jmeno || ' uvedl podozrivy kontakt ' || odkaz.kontakt);
      END IF;
  END LOOP;
  CLOSE zamestnanci;
  EXCEPTION
  WHEN OTHERS THEN
    raise_application_error(-1, 'Chyba pri kontrole jmen.');
END;
/


-- Add values into tables
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('Shizuoka', 'slunecno, teplo', 'hornata krajina, kysela puda');
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('Miyazaki', 'vlhko, teplo', 'urodna puda');
INSERT INTO Oblast (nazev_oblast, charakteristika, popis) VALUES('DongTing', 'vlhko, cisto', 'hornata krajina');

INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Japonsko', 'Asie');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Cejlon', 'Asie');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Maroko', 'Afrika');
INSERT INTO Zeme (nazev_zeme, kontinent) VALUES('Norsko', 'Evropa');

INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(NULL, 'Sencha Shizu', 'Green', 'Shizu 1732', 3, 'TGFOP', 'Japonsko', 'Shizuoka');
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(2, 'Ceylon High Grown', 'Black', 'Manutea', 5, 'OP', 'Cejlon', NULL);
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(3, 'Gyokuro Yabukita', 'Green', 'Yabukita', 2, 'FOP', 'Japonsko', 'Miyazaki');
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(NULL, 'Gyokuro Yamaguchi', 'Green', 'Yabukita', 3, 'GFOP', 'Japonsko', 'Miyazaki');
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(NULL, 'Tuareg', 'Herb', 'Maroko tea', 6, NULL, 'Maroko', NULL);
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(6, 'Tuareg extra', 'Herb', 'Maroko tea', 5, NULL, 'Maroko', NULL);
INSERT INTO Caj (id, nazev, druh, dodavatel, doba_louhovani, kvalita, zeme_puvodu, nazev_oblasti) VALUES(7, 'Pi Lo Chun', 'Green', 'Tteeaa', 2, 'TGFOP', 'Cina', 'DongTing');

INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(1, 3, 140, 1);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(2, 4, 110, 2);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(3, 10, 90, 3);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(4, 8, 75, 4);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(5, 12, 60, 5);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(6, 10, 85, 6);
INSERT INTO Varka (id, mnozstvi, cena_100g, id_caj) VALUES(7, 9, 135, 7);


INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (1, 'Muhamad Ali', 'Cejl 2, Brno', date '2010-02-15', '9185261725', 'muhamad', '0178268a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat) VALUES (1, 'skladnik', 'muz', '0080/000152', 20000);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (2, 'Fero Malachosky', 'Nova 4, Bratislava', date '1992-01-20', '185019275', 'ferino', '0z78c68a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (2, 'prodejce', 'muz', '0080/250152', 28000, 1);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (3, 'Petr Janak', 'Govskeho 8, Praha', date '1990-02-28', '+420158623725', 'malach', '07556xycy');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat) VALUES (3, 'vykonny reditel', 'muz', '4080/080152', 12000);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (4, 'Johanna Ostrovskaja', 'Davacnego 2, Moskva', date '1989-01-01', '+185622325412', 'xjohann01', '1926234a');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (4, 'recepcni', 'zena', '800050/526152', 15000, 1);
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (5, 'Jana Modra', 'Davacnego 2, Moskva', date '1979-11-11', '+185603754922', 'xmodra01', '73hgd7354');
INSERT INTO Zamestnanec (id, pozice, pohlavi, cislo_uctu, plat, id_veduci) VALUES (5, 'skladnik', 'zena', '7961114447', 11000, 4);

INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (6, 'Microsoft', '5th NewStreet, New York', null, '+418252622375', 'mcrsft', '189162345');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (6, 0, date '1960-10-10');
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (7, 'Redhat', 'Technologicky Park, Brno', null, '911582823', 'rhos123', '123456789');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (7, 189, date '1980-11-12');
INSERT INTO Osoba (id, jmeno, adresa, datum_narozeni, kontakt, login, heslo) VALUES (8, 'John Clinton', 'Gajdosova 1, Brno', date '1976-05-02', '018926523', 'clinton1', '08656ds0');
INSERT INTO Odberatel (id, vernostni_body, datum_registrace) VALUES (8, 4230, date '1989-10-12');

INSERT INTO Faktura (id, cena) VALUES (1, 180000);
INSERT INTO Faktura (id, cena) VALUES (2, 256);
INSERT INTO Faktura (id, cena) VALUES (3, 4000);

INSERT INTO Objednavka (id, stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES (1, 'done', date '1985-05-02', 1, 6);
INSERT INTO Objednavka (id, stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES (2, 'pending', date '1987-01-02', 2, 7);
INSERT INTO Objednavka (id, stav, datum_vytvoreni, id_faktura, id_odberatel) VALUES (3, 'done', date '2016-03-02', 3, 7);

INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (1, 500, 15600, 1, 1);
INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (2, 8800, 100, 2, 2);
INSERT INTO Obsahuje (id, mnozstvi, cena, id_objednavka, id_varka) VALUES (3, 300, 125, 3, 4);

INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (1, 'Druha 5, Uherske Hradiste', 94956, 1, 1);
INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (2, 'Moja ulica 1, Nitra', 94901, 2, 2);
INSERT INTO Vyrizuje (id, adresa, sledovaci_cislo, id_zamestnanec, id_objednavka) VALUES (3, 'Moja ulica 1, Nitra', 94932, 2, 3);

-- UKAZKA PROCEDURY PREMIE
-- Stav pred zvysovanim odmen
SELECT Z.id, O.jmeno, Z.plat FROM Zamestnanec Z LEFT JOIN Osoba O ON Z.id = O.id;
-- Pridani odmeny (Zvyseni platu) zamestnanci, ktery splnuje vsechny podminky
EXEC premie(10, 2);
-- Pridani odmeny (Zvyseni platu) zamestnanci, ktery nesplnuje podminky (vyjimka)
EXEC premie(10, 4);
-- Stav po zvysovani odmen
SELECT Z.id, O.jmeno, Z.plat FROM Zamestnanec Z LEFT JOIN Osoba O ON Z.id = O.id;

-- Kontrola podozrivych kontaktu zamestnancu
EXEC podozrive_kontakty();

-- POUZITI EXPLAIN PLAN
-- DROP INDEX OSindex; -- Pokud se potrebuje resetovat
EXPLAIN PLAN FOR
SELECT Z.nazev_zeme, SUM(V.mnozstvi) AS Celkove_mnozstvi
  FROM Varka V, Caj C, Zeme Z
  WHERE V.id_caj = C.id AND C.zeme_puvodu = Z.nazev_zeme AND V.cena_100g > 85
  GROUP BY nazev_zeme;
SELECT * FROM TABLE(DBMS_XPLAN.display);

CREATE INDEX OSindex ON Varka(cena_100g);
EXPLAIN PLAN FOR
SELECT /*+ INDEX(OSindex) */Z.nazev_zeme, SUM(V.mnozstvi) AS Celkove_mnozstvi
  FROM Varka V, Caj C, Zeme Z
  WHERE V.id_caj = C.id AND C.zeme_puvodu = Z.nazev_zeme AND V.cena_100g > 85
  GROUP BY nazev_zeme;
SELECT * FROM TABLE(DBMS_XPLAN.display);

-- PRIDANI PRAV DRUHEMU CLENU TYMU
-- tabulky
GRANT ALL ON Zamestnanec TO xhraba12;
GRANT ALL ON Osoba TO xhraba12;
GRANT ALL ON Vyrizuje TO xhraba12;
GRANT ALL ON Objednavka TO xhraba12;
GRANT ALL ON Objednavka TO xhraba12;

-- procedury
GRANT EXECUTE ON premie TO xhraba12;
GRANT EXECUTE ON podozrive_kontakty TO xhraba12;


-- Materializovany pohled
DROP MATERIALIZED VIEW faktury;

CREATE MATERIALIZED VIEW faktury
CACHE
BUILD IMMEDIATE
REFRESH ON COMMIT
AS
  SELECT id, cena FROM Faktura;

-- garantovani prav
GRANT SELECT ON faktury TO xhraba12;

-- ukazky nalezeni polozek, jsou stejne
SELECT id, cena FROM Faktura;
SELECT id, cena FROM faktury;

-- modifikace prvni tabulky
INSERT INTO Faktura (id, cena) VALUES (4, 5600);
COMMIT;

-- polozky se aktualizovali
SELECT id, cena FROM Faktura;
SELECT id, cena FROM faktury;
