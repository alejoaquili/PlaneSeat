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
PRIMARY KEY(flightNumber, rowLetter, colNumber),
FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);


CREATE TABLE reservation(
flightNumber 	TEXT,
userId 			TEXT,
rowLetter 		CHAR(1) NOT NULL,
colNumber 		INTEGER NOT NULL,
PRIMARY KEY(flightNumber, userId, rowLetter, colNumber),
FOREIGN KEY(flightNumber) REFERENCES flight ON DELETE CASCADE);