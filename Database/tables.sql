CREATE TABLE flight(
flightNumber 	TEXT,
origin 			TEXT,
destination 	TEXT,
freeSeats 		INTEGER,
PRIMARY KEY(flightNumber));


CREATE TABLE flightSeats(
flightNumber 	TEXT,
rowLetter    	CHAR(1),
colNumber  		INTEGER,
occupied		BOOLEAN,
FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE SET NULL);


CREATE TABLE reservation(
flightNumber 	TEXT,
userId 			TEXT,
rowLetter 		CHAR(1) NOT NULL,
colNumber 		INTEGER NOT NULL,
FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE SET NULL),
UNIQUE(flightNumber, userId, rowLetter, colNumber));

