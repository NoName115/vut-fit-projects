--zmazanie tabuliek
DROP TABLE Zmluva  CASCADE CONSTRAINTS;

DROP TABLE Pracovnik   CASCADE CONSTRAINTS;

DROP TABLE Stretnutie   CASCADE CONSTRAINTS;

DROP TABLE SpecializujeSa  CASCADE CONSTRAINTS;

DROP TABLE Znacka  CASCADE CONSTRAINTS ;

DROP TABLE Oblecenie   CASCADE CONSTRAINTS;

DROP TABLE Obsahuje   CASCADE CONSTRAINTS;

DROP TABLE Zakaznik   CASCADE CONSTRAINTS;

DROP TABLE FyzickaOsoba  CASCADE CONSTRAINTS;

DROP TABLE PravnickaOsoba  CASCADE CONSTRAINTS;

--zmazanie indexu
DROP INDEX BydliskoIndex;
DROP INDEX BydliskoIndex_2;
-- zmazanie MV
DROP MATERIALIZED VIEW MaterView;

--zmazanie procedury
DROP PROCEDURE calculate_sum;

-- zmazanie sequencie
DROP SEQUENCE pracovnikSequence;
DROP SEQUENCE znackaSequence;
DROP SEQUENCE oblecenieSequence;
DROP SEQUENCE zakaznikSequence;
DROP SEQUENCE zmluvaSequence;
DROP SEQUENCE stretnutieSequence;

-- vytvorenie sequencie
CREATE SEQUENCE pracovnikSequence START with 1 INCREMENT BY 1 NOMAXVALUE;
CREATE SEQUENCE znackaSequence START with 1 INCREMENT BY 1 NOMAXVALUE;
CREATE SEQUENCE oblecenieSequence START with 1 INCREMENT BY 1 NOMAXVALUE;
CREATE SEQUENCE zakaznikSequence START with 1 INCREMENT BY 1 NOMAXVALUE;
CREATE SEQUENCE zmluvaSequence START with 1 INCREMENT BY 1 NOMAXVALUE;
CREATE SEQUENCE stretnutieSequence START with 1 INCREMENT BY 1 NOMAXVALUE;

alter session set nls_date_format = 'DD.MON.YYYY';

CREATE TABLE Pracovnik (
OsobneCislo INTEGER PRIMARY KEY,
meno VARCHAR(20) NOT NULL,
priezvisko VARCHAR(20) NOT NULL,
datumNarodenia DATE NOT NULL
);

CREATE TABLE Znacka (
idZnacky INTEGER PRIMARY KEY,
navrhar VARCHAR(25)
);

CREATE TABLE Oblecenie (
IdOblecenia  INTEGER PRIMARY KEY,
nazov VARCHAR(25),
popis VARCHAR(50),
farba VARCHAR(15),
velkost INTEGER,
cenaZaKus INTEGER,
--FK
ZNACKA INTEGER,
FOREIGN KEY (ZNACKA) REFERENCES Znacka
);

CREATE TABLE Zakaznik (
OsobneCislo INTEGER PRIMARY KEY,
mestoBydliska VARCHAR(30) NOT NULL,
ulicaBydliska VARCHAR(30) NOT NULL,
cisloDomuBydliska INTEGER NOT NULL,
email VARCHAR(30),
telefonneCislo INTEGER,
OC_PRACOVNIK INTEGER,
FOREIGN KEY  (OC_PRACOVNIK) REFERENCES Pracovnik
);

CREATE TABLE Zmluva (
idZmluvy  INTEGER PRIMARY KEY,
celkovaCena INTEGER,
mestoDorucenia VARCHAR(30) NOT NULL,
ulicaDorucenia VARCHAR(30) NOT NULL,
cisloDomuDorucenia INTEGER NOT NULL,
cisloUctuIBAN VARCHAR(24),
--FK
OC_PRACOVNIK INTEGER ,
OC_ZAKAZNIK INTEGER ,
FOREIGN KEY  (OC_PRACOVNIK) REFERENCES Pracovnik,
FOREIGN KEY  (OC_ZAKAZNIK) REFERENCES Zakaznik
);

CREATE TABLE Stretnutie (
IdStretnutia INTEGER PRIMARY KEY,
datum DATE NOT NULL,
hodina INTEGER,
minuta INTEGER,
popis VARCHAR(60),
CHECK (hodina < 24),
CHECK (minuta < 60),
-- FK
OC_ZAKAZNIK INTEGER,
OC_PRACOVNIK INTEGER,
FOREIGN KEY  (OC_ZAKAZNIK) REFERENCES Zakaznik,
FOREIGN KEY  (OC_PRACOVNIK) REFERENCES Pracovnik
);

CREATE TABLE FyzickaOsoba(
rodneCislo VARCHAR(10) PRIMARY KEY, -- doplnit kontrolu
menoFyzOsoby VARCHAR(20) NOT NULL,
priezviskoFyzOsoby VARCHAR(20) NOT NULL,
--FK
OC_ZAKAZNIK INTEGER ,
FOREIGN KEY  (OC_ZAKAZNIK) REFERENCES Zakaznik
);

CREATE TABLE PravnickaOsoba(
ico INTEGER PRIMARY KEY,
nazov VARCHAR(30) NOT NULL UNIQUE,
--FK
OC_ZAKAZNIK INTEGER,
RC_FYZ_OSOBA VARCHAR(10),
FOREIGN KEY (OC_ZAKAZNIK) REFERENCES Zakaznik,
FOREIGN KEY (RC_FYZ_OSOBA) REFERENCES FyzickaOsoba
);

CREATE TABLE SpecializujeSa (
FK_znacka INTEGER,
FK_pracovnik INTEGER,
FOREIGN KEY (FK_pracovnik) REFERENCES Pracovnik,
FOREIGN KEY (FK_znacka) REFERENCES Znacka
);

CREATE TABLE Obsahuje(
pocetKusov INTEGER NOT NULL,
FK_oblecenie INTEGER,
FK_zmluva INTEGER,
FOREIGN KEY (FK_oblecenie) REFERENCES Oblecenie,
FOREIGN KEY (FK_zmluva) REFERENCES Zmluva
);

ALTER TABLE SpecializujeSa ADD CONSTRAINT PK_spec PRIMARY KEY (FK_znacka, FK_pracovnik);
ALTER TABLE Obsahuje ADD CONSTRAINT PK_obs PRIMARY KEY (FK_oblecenie, FK_zmluva);


-- triger pre kontrolu rodneho cisla v tabulke FyzickaOsoba
SET serveroutput ON;

CREATE OR REPLACE TRIGGER RC_trigger
  BEFORE INSERT OR UPDATE OF rodneCislo ON FyzickaOsoba
  FOR EACH ROW
DECLARE
  RC VARCHAR(10);
  den NUMBER;
  mesiac NUMBER;
  rok NUMBER;
  lomitko NUMBER;
  RC_plus BOOLEAN;
  ECP BOOLEAN;

BEGIN
  RC := :NEW.rodneCislo;
  RC_plus := FALSE;
  ECP := FALSE;

  rok :=    TO_NUMBER( SUBSTR(RC, 1, 2) ,'99');
  mesiac := TO_NUMBER( SUBSTR(RC, 3, 2) ,'99');
  den :=    TO_NUMBER( SUBSTR(RC, 5, 2) ,'99');

  IF ( LENGTH(RC)  = 9) THEN
    lomitko := TO_NUMBER( SUBSTR(RC, 7, 3) ,'999');
    IF ( lomitko = 0 ) THEN
      Raise_Application_Error (-25000, 'Nulova koncovka u 9-miestneho rodneho cisla');
    END IF;

  ELSIF ( LENGTH(RC)  = 10) THEN
    lomitko := TO_NUMBER( SUBSTR(RC, 7, 4) ,'9999');
    IF ( MOD( (rok * 100000000) + (mesiac * 1000000) + (den * 10000) + lomitko, 11 ) != 0 ) THEN
      Raise_Application_Error (-25000, '10 mieste rodne cislo nieje delitelne 11');
    END IF;

  END IF;

  --zena
  IF ( mesiac > 50 ) THEN
    mesiac := mesiac - 50;
  END IF;


  IF ( mesiac > 20 ) THEN
    mesiac := mesiac - 20;
    RC_plus := TRUE;
  END IF;

  IF ( den > 40 ) THEN
    den := den - 40;
    ECP := TRUE;
  END IF;

  IF (( RC_plus = TRUE ) AND (ECP = TRUE )) THEN
    Raise_Application_Error (-25000, 'Nevalidny forat RC');
  END IF;


  IF (( mesiac < 0) OR (mesiac > 12)) THEN
    Raise_Application_Error (-20002, 'neplatny mesiac v rodnom cisle!');
  END IF;

  --TODO check date ... den v mesiaci musi vyhovovat feb28 .. 31/30

  IF (( den < 0) OR (den > 31)) THEN
    Raise_Application_Error (-20002, 'neplatn� den v rodnom cisle!');
  END IF;

END RC_trigger;
/
show errors

-- triger pre autoinkrementaciu Osobneho cisla Zakaznika
CREATE OR REPLACE TRIGGER onInsertZakaznik
	BEFORE INSERT ON Zakaznik
    FOR EACH ROW
WHEN (new.OsobneCislo IS NULL)
BEGIN
    SELECT zakaznikSequence.nextval INTO :new.OsobneCislo FROM DUAL;
END;
/

-- triger pre autoinkrementaciu Osobneho cisla Pracovnika
CREATE OR REPLACE TRIGGER onInsertPracovnik
	BEFORE INSERT ON Pracovnik
    FOR EACH ROW
WHEN (new.OsobneCislo IS NULL)
BEGIN
    SELECT pracovnikSequence.nextval INTO :new.OsobneCislo FROM DUAL;
END;
/

-- triger pre autoinkrementaciu ID znacky
CREATE OR REPLACE TRIGGER onInsertZnacka
	BEFORE INSERT ON Znacka
    FOR EACH ROW
WHEN (new.idZnacky IS NULL)
BEGIN
    SELECT znackaSequence.nextval INTO :new.idZnacky FROM DUAL;
END;
/

-- triger pre autoinkrementaciu ID oblecenia
CREATE OR REPLACE TRIGGER onInsertOblecenie
	BEFORE INSERT ON Oblecenie
    FOR EACH ROW
WHEN (new.idOblecenia IS NULL)
BEGIN
    SELECT oblecenieSequence.nextval INTO :new.idOblecenia FROM DUAL;
END;
/
-- triger pre autoinkrementaciu ID zmluvy
CREATE OR REPLACE TRIGGER onInsertZmluva
	BEFORE INSERT ON Zmluva
    FOR EACH ROW
WHEN (new.idZmluvy IS NULL)
BEGIN
    SELECT zmluvaSequence.nextval INTO :new.idZmluvy FROM DUAL;
END;
/

-- triger pre autoinkrementaciu ID stretnutia
CREATE OR REPLACE TRIGGER onInsertStretnutie
	BEFORE INSERT ON Stretnutie
    FOR EACH ROW
WHEN (new.idStretnutia IS NULL)
BEGIN
    SELECT stretnutieSequence.nextval INTO :new.idStretnutia FROM DUAL;
END;
/

-- zrusenie a nasledne vytvorenie procedury ktora spocita celkovu cenu produktov danej znacky
DROP PROCEDURE SUCET_CENY_PRODUKTOV;

CREATE OR REPLACE PROCEDURE SUCET_CENY_PRODUKTOV(nazov_znacky znacka.idznacky%TYPE)
IS
  ret znacka.idznacky%TYPE := 0;
  cenicka OBLECENIE.CENAZAKUS%TYPE;
   CURSOR kurzor IS
    SELECT O.CENAZAKUS FROM OBLECENIE O, ZNACKA Z WHERE Z.IDZNACKY = nazov_znacky AND Z.IDZNACKY = O.ZNACKA;
BEGIN
  OPEN kurzor;
  LOOP
    FETCH kurzor INTO cenicka;
    EXIT WHEN kurzor%NOTFOUND;
    ret := ret + cenicka;
  END LOOP;
  DBMS_OUTPUT.put_line('celkova cena produktov danej znacky je ' || ret);
END;   
/
show errors


 -- procedura na vypocet celkovej ceny  zmluvy a danu hodnotu updatuje
DROP PROCEDURE VYPOCITAT_SUMA;

CREATE OR REPLACE PROCEDURE VYPOCITAT_SUMA(id_zmluvy zmluva.idZmluvy%TYPE)
IS
  vysledok zmluva.celkovaCena%TYPE := 0;
  cena INTEGER;
  pocet INTEGER;
  no_exist EXCEPTION;
  CURSOR kurzor IS
    SELECT obsahuj.pocetKusov, obl.CENAZAKUS FROM OBSAHUJE obsahuj, ZMLUVA Z, OBLECENIE obl WHERE Z.IDZMLUVY = id_zmluvy AND Z.IDZMLUVY = obsahuj.FK_Zmluva AND obl.idOblecenia = obsahuj.FK_OBLECENIE;
BEGIN
  OPEN kurzor;
  LOOP
    FETCH kurzor INTO cena, pocet;
    EXIT WHEN kurzor%NOTFOUND;
    vysledok := vysledok + cena * pocet;
  END LOOP;
  
  IF vysledok = 0 THEN
    RAISE no_exist;
  END IF;
  
  UPDATE ZMLUVA SET celkovaCena = vysledok WHERE idZmluvy = id_zmluvy;
  COMMIT;
  CLOSE kurzor;

EXCEPTION 
  WHEN no_exist THEN
    DBMS_OUTPUT.put_line('Zmluva neobsahuje produkty');
END; 
/
show errors

/*bolo pouzite pre SELECTY*/
-- vztahuje sa k nahodnym datam ktorymi su naplnene tabulky (pre zvysenie prehladnosti)
--Pracovnici PK od 1 - 20 ; Znacka PK od 440 do 460 ; Oblecenie PK od 20 do 40 ; Zakaznik  PK od 1200 do 1220 ; Zmluva PK od 240 do 260 ;
-- Stretnutie PK od 800 do 820 ;
INSERT INTO Pracovnik
VALUES(pracovnikSequence.nextval, 'Dusan', 'Varga', TO_DATE('10.12.1964', 'dd.mm.yyyy'));
INSERT INTO Pracovnik
VALUES(pracovnikSequence.nextval, 'Lukas', 'Zahorak', TO_DATE('10.12.1984', 'dd.mm.yyyy'));
INSERT INTO Pracovnik
VALUES(pracovnikSequence.nextval, 'Marek', 'Fico', TO_DATE('24.11.1969', 'dd.mm.yyyy'));
INSERT INTO Pracovnik
VALUES(pracovnikSequence.nextval, 'Ondrej', 'Kurak', TO_DATE('24.12.1972', 'dd.mm.yyyy'));
INSERT INTO Pracovnik
VALUES(NULL, 'Eduard', 'Cuba', TO_DATE('1.10.1980', 'dd.mm.yyyy'));

INSERT INTO Znacka
VALUES(440,'Guci');
INSERT INTO Znacka
VALUES(441,'Helfinger');
INSERT INTO Znacka
VALUES(442,'Georgio Armani');
INSERT INTO Znacka
VALUES(443,'Coco Chanel');
INSERT INTO Znacka
VALUES(444,'Calvin Klein');

INSERT INTO Oblecenie
VALUES(20, 'nohavice', '100% bavlna', 'cervena', 40, 400, 444);
INSERT INTO Oblecenie
VALUES(21, 'nohavice', '100% bavlna', 'cervena', 42, 380, 444);
INSERT INTO Oblecenie
VALUES(22, 'nohavice', '100% bavlna', 'cervena', 44, 390, 444);
INSERT INTO Oblecenie
VALUES(23, 'nohavice', '100% bavlna', 'modra', 44, 400, 444);
INSERT INTO Oblecenie
VALUES(24, 'ciapka', 'nova kolekcia', 'fialova', 40, 1199, 443);
INSERT INTO Oblecenie
VALUES(25, 'rukavice', '100% bavlna', 'bordova', 42, 199, 442);
INSERT INTO Oblecenie
VALUES(26, 'kosela', 'nova kolekcia', 'zelena', 46, 599, 441);
INSERT INTO Oblecenie
VALUES(27, 'tricko', '100% saten', 'ruzova', 48, 1200, 441);
INSERT INTO Oblecenie
VALUES(28, 'sandaly', 'nova kolekcia, hlinikove pracky', 'oranzova', 48, 629, 440);
INSERT INTO Oblecenie
VALUES(29, 'ponozky', 'bambusove', 'modra', 50, 59, 440);
INSERT INTO Oblecenie
VALUES(30, 'Opasok', 'Krokodil koza', 'oranzova', 50, 5999, 440);

EXECUTE SUCET_CENY_PRODUKTOV(441);

INSERT INTO Zakaznik
VALUES(1200, 'Brno', 'Bozetchova', 18, 'forgac@gmail.com', '0948231978', 1);
INSERT INTO Zakaznik
VALUES(1201, 'Praha', 'Bozetchova', 44, 'hric@gmail.com', '0949753979', 1);
INSERT INTO Zakaznik
VALUES(1202, 'Ostrava', 'Halaskova', 20, 'peha@peha.com', '0950824132', 1); --firma
INSERT INTO Zakaznik
VALUES(1203, 'Presov', 'Sibirska', 18, 'maramed@maramed.com', '0950824131', 2); --firma
INSERT INTO Zakaznik
VALUES(1204, 'Presov', 'Gorkeho', 256, 'jusko@gmail.com', '0989824132', 3);
INSERT INTO Zakaznik
VALUES(1205, 'Kosice', 'Jablonova', 64, 'hrib@gmail.com', '095464132', 4);
INSERT INTO Zakaznik
VALUES(1206, 'Presov', 'Mukacevska', 128, 'Fico@gmail.com', '0923497361', 2);

INSERT INTO Zmluva
VALUES('240', null, 'Brno', 'Bozetchova', 18, 'SK6807200002894447426353',1,1200);

EXECUTE VYPOCITAT_SUMA(240);

INSERT INTO Zmluva
VALUES('242', '5999', 'Presov', 'Gorkeho', 256,'SK6807200002891985526353',1,1204);
INSERT INTO Zmluva
VALUES('243', '2378', 'Presov', 'Mukacevska', 128, 'SK6807200002891985526353',2,1206);
INSERT INTO Zmluva
VALUES('244', '2578', 'Brno', 'Hercikova', 45, 'SK6807200002891985526353',3,1202); -- firma s inou dodacou adresou

INSERT INTO Stretnutie
VALUES(800, TO_DATE('10.4.2017', 'dd.mm.yyyy'), 16, 40, NULL, 1200, 1);
INSERT INTO Stretnutie
VALUES(801, TO_DATE('10.5.2017', 'dd.mm.yyyy'), 16, 40, NULL, 1201, 1);
INSERT INTO Stretnutie
VALUES(802, TO_DATE('20.6.2017', 'dd.mm.yyyy'), 18, 45, NULL, 1202, 1);
INSERT INTO Stretnutie
VALUES(803, TO_DATE('24.6.2017', 'dd.mm.yyyy'), 10, 00, 'Podpis Zmluvy', 1203, 2);
INSERT INTO Stretnutie
VALUES(804, TO_DATE('24.6.2017', 'dd.mm.yyyy'), 13, 15, 'Reklamacia', 1204, 3);
INSERT INTO Stretnutie
VALUES(805, TO_DATE('25.6.2017', 'dd.mm.yyyy'), 14, 35, 'Reklamacia', 1202, 3);
INSERT INTO Stretnutie
VALUES(806, TO_DATE('26.6.2017', 'dd.mm.yyyy'), 20, 00, NULL, 1203, 4);


INSERT INTO FyzickaOsoba
VALUES(9601317693,'Michal', 'Forgac', 1200);
INSERT INTO FyzickaOsoba
VALUES(9653217717,'Juraj', 'Jusko', 1204);
INSERT INTO FyzickaOsoba
VALUES(9501317694,'Adam', 'Hric', 1201);
INSERT INTO FyzickaOsoba
VALUES(9401317695,'Ondrej', 'Hrib', 1205);
INSERT INTO FyzickaOsoba
VALUES(9301317696,'Martin', 'Fico', 1206);

INSERT INTO PravnickaOsoba
VALUES(12345679, 'peha s.r.o.', 1202, 9601317693);
INSERT INTO PravnickaOsoba
VALUES(12345678, 'maramed s.r.o.', 1203, NULL);

INSERT INTO SpecializujeSa
VALUES(440,1);
INSERT INTO SpecializujeSa
VALUES(440,2);
INSERT INTO SpecializujeSa
VALUES(440,3);
INSERT INTO SpecializujeSa
VALUES(441,4);
INSERT INTO SpecializujeSa
VALUES(441,5);
INSERT INTO SpecializujeSa
VALUES(442,1);
INSERT INTO SpecializujeSa
VALUES(443,2);
INSERT INTO SpecializujeSa
VALUES(443,1);
INSERT INTO SpecializujeSa
VALUES(444,1);

-- 1 zmluva obsahujuca 3 rozne typy oblecenia
INSERT INTO Obsahuje
VALUES(1,20,240);
INSERT INTO Obsahuje
VALUES(25,21,240);
INSERT INTO Obsahuje
VALUES(15,22,240);

INSERT INTO Obsahuje
VALUES(100,20,242);
INSERT INTO Obsahuje
VALUES(210,20,243);
INSERT INTO Obsahuje
VALUES(10000,20,244);


--pridelenie prav 
GRANT ALL ON Zmluva TO xkolcu00;
GRANT ALL ON Pracovnik TO xkolcu00;
GRANT ALL ON Stretnutie TO xkolcu00;
GRANT ALL ON SpecializujeSa TO xkolcu00;
GRANT ALL ON Znacka TO xkolcu00;
GRANT ALL ON Oblecenie TO xkolcu00;
GRANT ALL ON Obsahuje TO xkolcu00;
GRANT ALL ON Zakaznik TO xkolcu00;
GRANT ALL ON FyzickaOsoba TO xkolcu00;
GRANT ALL ON PravnickaOsoba TO xkolcu00;


create materialized view MaterView
nologging
cache
build immediate
enable query rewrite
as
	SELECT O.cenaZaKus, O.nazov FROM OBLECENIE O, ZMLUVA Z, OBSAHUJE OB WHERE Z.IDZMLUVY = 240 AND OB.FK_ZMLUVA = 240 AND OB.FK_OBLECENIE = O.IDOBLECENIA;

SELECT * FROM MaterView;

--EXPLAIN PLAN
EXPLAIN PLAN FOR SELECT COUNT(*) AS pocet_zmluv, za.osobnecislo, za.mestoBydliska FROM zmluva z, zakaznik za
	WHERE za.mestoBydliska = 'Presov' AND za.mestoBydliska = z.mestoDorucenia
	GROUP BY za.osobneCislo, za.mestoBydliska;
SELECT plan_table_output FROM TABLE(dbms_xplan.display());


CREATE INDEX BydliskoIndex_2 ON ZMLUVA(mestoDorucenia);
CREATE INDEX BydliskoIndex ON ZAKAZNIK(mestoBydliska);

EXPLAIN PLAN FOR SELECT COUNT(*) AS pocet_zmluv, za.osobnecislo, za.mestoBydliska FROM zmluva z, zakaznik za
	WHERE za.mestoBydliska = 'Presov' AND za.mestoBydliska = z.mestoDorucenia
	GROUP BY za.osobneCislo, za.mestoBydliska;
SELECT plan_table_output FROM TABLE(dbms_xplan.display());

 -- vypis tabuliek
/*SELECT * FROM PRACOVNIK;
SELECT * FROM ZNACKA;
SELECT * FROM OBLECENIE;
SELECT * FROM OBSAHUJE;
SELECT * FROM SPECIALIZUJESA;
SELECT * FROM PRAVNICKAOSOBA;
SELECT * FROM FYZICKAOSOBA;
SELECT * FROM STRETNUTIE;
SELECT * FROM ZMLUVA;
SELECT * FROM ZAKAZNIK;*/

-- vyp��e v�etky zmluvy ku konkr�tnemu z�kazn�kovi
SELECT Z.* FROM ZMLUVA Z, ZAKAZNIK Za WHERE Z.OC_ZAKAZNIK = Za.OsobneCislo AND Za.osobneCislo = 1200;
SELECT Z.* FROM ZMLUVA Z, ZAKAZNIK Za WHERE Z.OC_ZAKAZNIK = Za.OsobneCislo AND Za.osobneCislo = 1202;
--  meno a priezvisko  v�etk�ch fyzick�ch os�b,  ktor� zastupuj� aspo� jednu pr�vnick� osobu
SELECT F.menoFyzOsoby, F.priezviskoFyzOsoby FROM FYZICKAOSOBA F, PRAVNICKAOSOBA P WHERE P.RC_FYZ_OSOBA = F.RODNECISLO;
-- vyp�sa� v�etky typy oble�enia ( nazov a cena za kus), ktor� obsahuje zmluva
SELECT O.cenaZaKus, O.nazov FROM OBLECENIE O, ZMLUVA Z, OBSAHUJE OB WHERE Z.IDZMLUVY = 240 AND OB.FK_ZMLUVA = 240 AND OB.FK_OBLECENIE = O.IDOBLECENIA;
--  v�etci pracovn�ci ktor� mali aspo� 2 stretnutia + pocet kolko ich mali
SELECT P.OSOBNECISLO, P.MENO, P.PRIEZVISKO ,COUNT(*) FROM PRACOVNIK P, STRETNUTIE S  WHERE P.OSOBNECISLO = S.OC_PRACOVNIK  GROUP BY P.OSOBNECISLO, P.MENO, P.PRIEZVISKO HAVING COUNT(*) > 1;
-- ko�ko zml�v bolo odoslan�ch do Presova
SELECT COUNT(*) FROM ZMLUVA Z WHERE Z.mestoDorucenia = 'Presov';
--kolko klientov maju jednotlivy pracovnici
SELECT COUNT(*),P.MENO, P.PRIEZVISKO FROM PRACOVNIK P, ZAKAZNIK Z WHERE Z.OC_PRACOVNIK = P.OSOBNECISLO GROUP BY P.MENO, P.PRIEZVISKO;
--ktory pracovnik ma rovnake priezvisko ako klient ?
SELECT P.* FROM Pracovnik P WHERE P.PRIEZVISKO IN (SELECT priezviskoFyzOsoby FROM FYZICKAOSOBA);
-- ake maily  maju zakaznici ktori maju stretnutie iba s pracovnikom  Dusan Varga (OC 1)
SELECT Z.EMAIL FROM  ZAKAZNIK Z, STRETNUTIE S WHERE S.OC_PRACOVNIK = 1 AND S.OC_ZAKAZNIK = Z.OSOBNECISLO
AND  NOT EXISTS (SELECT Z.EMAIL FROM  STRETNUTIE SA WHERE SA.OC_PRACOVNIK <> 1 AND SA.OC_ZAKAZNIK = Z.OSOBNECISLO);
