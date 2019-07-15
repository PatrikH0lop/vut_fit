SET foreign_key_checks = 0;

DROP TABLE IF EXISTS asset;
DROP TABLE IF EXISTS administrator;
DROP TABLE IF EXISTS employee;
DROP TABLE IF EXISTS room;
DROP TABLE IF EXISTS employeeReport;

SET foreign_key_checks = 1;

CREATE TABLE room (
  id INTEGER NOT NULL AUTO_INCREMENT,
  name VARCHAR(25) NOT NULL,
  building VARCHAR(25) NOT NULL,
  floor VARCHAR(25) NOT NULL,
  description VARCHAR(25),
  PRIMARY KEY(id)
);

CREATE TABLE employee (
  id INTEGER NOT NULL AUTO_INCREMENT,
  first_name VARCHAR(20) NOT NULL,
  last_name VARCHAR(20) NOT NULL,
  login VARCHAR(20) NOT NULL,
  password VARCHAR(20) NOT NULL,
  id_room INTEGER NOT NULL,
  FOREIGN KEY (id_room) REFERENCES room (id) ON DELETE CASCADE,
  PRIMARY KEY(id)
);

CREATE TABLE administrator (
  id INTEGER NOT NULL AUTO_INCREMENT,
  id_employee INTEGER NOT NULL,
  FOREIGN KEY (id_employee) REFERENCES employee (id) ON DELETE CASCADE,
  PRIMARY KEY(id)
);

CREATE TABLE asset (
  id INTEGER NOT NULL AUTO_INCREMENT,
  type VARCHAR(35) NOT NULL,
  name VARCHAR(35) NOT NULL,
  description VARCHAR(25) NOT NULL,
  price INTEGER NOT NULL,
  date_of_evidence DATE NOT NULL,
  id_employee INTEGER,
  id_room INTEGER,
  FOREIGN KEY (id_employee) REFERENCES employee(id) ON DELETE CASCADE,
  FOREIGN KEY (id_room) REFERENCES room(id) ON DELETE CASCADE,
  PRIMARY KEY(id)
);

CREATE TABLE employeeReport (
  id INTEGER NOT NULL AUTO_INCREMENT,
  report_text VARCHAR(1000) NOT NULL,
  date Date,
  id_employee INTEGER,
  FOREIGN KEY (id_employee) REFERENCES employee(id) ON DELETE CASCADE,
  PRIMARY KEY(id)
);


INSERT INTO room (name, building, floor, description) VALUES ('Canteen', 'F20', 4, 'Canteen (cap. 400)');
INSERT INTO room (name, building, floor, description) VALUES ('Conference A03', 'F14', 2, 'Conference room (cap. 100)');
INSERT INTO room (name, building, floor, description) VALUES ('Meeting B02', 'F20', 1, 'Meeting room (cap. 3)');
INSERT INTO room (name, building, floor, description) VALUES ('Meeting B03', 'F20', 2, 'All hands meeting room (cap. 120)');
INSERT INTO room (name, building, floor, description) VALUES ('Office 1A', 'F20', 3, 'Main office');

INSERT INTO employee (first_name, last_name, login, password, id_room) VALUES('John', 'Mate', 'admin', 'admin', 5);
INSERT INTO employee (first_name, last_name, login, password, id_room) VALUES('Anabela', 'Blue', 'user', 'user', 5);
INSERT INTO employee (first_name, last_name, login, password, id_room) VALUES('Anna', 'Thread', 'employee3', 'employee3', 5);
INSERT INTO employee (first_name, last_name, login, password, id_room) VALUES('Martin', 'Malen', 'employee4', 'employee4', 5);

INSERT INTO administrator (id_employee) VALUES (1); 

INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('mouse', 'Razor 2018', 'CPS 200', 50, date '2005-05-02', 1, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('mouse', 'Razor 2016', 'CPS 300', 30, date '2003-05-02', null, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('keyboard', 'LG XZY', 'Ergonomic', 20, date '2006-05-02', null, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('keyboard', 'LG AVZ', 'Gaming', 25, date '2010-06-04', 2, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('monitor', 'LG V30', '45Hz, 24"', 500, date '2005-06-04', null, 1);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('monitor', 'Acer Nitro', '60Hz, 27"', 400, date '2012-02-05', null, 1);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('monitor', 'Acer Nitro', '60Hz, 27"', 400, date '2012-04-07', 1, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('notebook', 'Asus H204v2', '2.4GHz, 16GB Ram', 50, date '2005-05-02', 1, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('notebook', 'Razor H204v2', '2.4GHz, 4GB Ram', 50, date '2005-05-02', 2, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('notebook', 'Razor H204v3', '2.5GHz, 8GB Ram', 50, date '2005-05-02', null, 2);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('notebook', 'Razor H204v3', '2.5GHz, 8GB Ram', 50, date '2005-05-02', null, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('docking station', 'OEM USB dock', '', 250, date '2005-05-02', 2, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('docking station', 'OEM USB dock', '', 250, date '2005-05-02', 1, null);
INSERT INTO asset (type, name, description, price, date_of_evidence, id_employee, id_room) VALUES('GPU', 'NVIDIA Quadro P600 2GB', '', 250, date '2005-05-02', 1, null);
