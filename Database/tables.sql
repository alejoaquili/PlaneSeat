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


CREATE OR REPLACE FUNCTION convertTextTimeToInterval(usedTime TEXT) RETURNS INTERVAL 
AS $$

DECLARE          
    hoursIndex 	 INTEGER := 0;
    minutesIndex INTEGER := 0;
    secondsIndex INTEGER := 0;     

BEGIN 
        select REPLACE(usedTime, ' ', '') INTO usedTime;
        select POSITION('H' IN usedTime) INTO hoursIndex;
        select POSITION('MIN' IN usedTime) INTO minutesIndex;
        select POSITION('SEG' IN usedTime) INTO secondsIndex;
        
        RETURN CAST(LEFT(usedTime, hoursIndex-1) || ' hour' AS INTERVAL) 
	         + CAST(SUBSTRING(usedTime FROM hoursIndex+1 FOR minutesIndex - hoursIndex) || ' minutes' AS INTERVAL) 
	         + CAST(SUBSTRING(usedTime FROM minutesIndex+3 FOR secondsIndex - minutesIndex -2) || ' seconds' AS INTERVAL);                
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION migrationProblem1() RETURNS VOID 
AS $$

DECLARE 
    myCursor CURSOR FOR SELECT * FROM recorrido_importado 
    					WHERE id_usuario IS NOT NULL AND fecha_hora_retiro IS NOT NULL 
    					AND origen_estacion IS NOT NULL AND destino_estacion IS NOT NULL 
    					AND tiempo_uso IS NOT NULL AND tiempo_uso NOT LIKE '-%';
    usedTime INTERVAL;   
    it 		 RECORD;           
        
BEGIN        
    OPEN myCursor;

    LOOP
	    FETCH myCursor INTO it;
	    EXIT WHEN NOT FOUND; 
	    usedTime := convertTextTimeToInterval(it.tiempo_uso);
	    IF(usedTime > INTERVAL '0') THEN            
			INSERT INTO recorridoTemp VALUES(it.periodo, it.id_usuario, it.fecha_hora_retiro, it.origen_estacion, it.destino_estacion, usedTime);                                                          
		END IF;
	END LOOP;
	CLOSE myCursor;          
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION filter2(aUser recorridoTemp.usuario%TYPE, aDate recorridoTemp.fecha_hora_ret%TYPE) RETURNS VOID
AS $$

DECLARE 
    repetedTupples CURSOR FOR 
    	SELECT * FROM recorridoTemp
	    WHERE usuario = aUser AND fecha_hora_ret = aDate
	    ORDER BY fecha_hora_ret, tiempo_uso;
    tuppleOrder INT;
    repeted 	RECORD;

BEGIN
    tuppleOrder := 0;
    OPEN repetedTupples;

    LOOP
        FETCH repetedTupples INTO repeted;
        EXIT WHEN NOT FOUND;

        IF(tuppleOrder != 1) THEN
            DELETE FROM recorridoTemp
            WHERE periodo = repeted.periodo AND usuario = repeted.usuario AND fecha_hora_ret = repeted.fecha_hora_ret 
                  AND est_origen = repeted.est_origen AND est_destino = repeted.est_destino AND tiempo_uso = repeted.tiempo_uso;
        END IF;
        tuppleOrder = tuppleOrder+1;
    END LOOP;
    CLOSE repetedTupples;
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION migrationProblem2() RETURNS VOID
AS $$

DECLARE 
    myCursor CURSOR FOR 
    	SELECT DISTINCT usuario, fecha_hora_ret 
    	FROM recorridoTemp
    	GROUP BY usuario, fecha_hora_ret
    	HAVING COUNT(*) > 1;
    myRecord RECORD;

BEGIN
    OPEN myCursor;

    LOOP
        FETCH myCursor INTO myRecord;
        EXIT WHEN NOT FOUND;

        PERFORM filter2(myRecord.usuario, myRecord.fecha_hora_ret);
    END LOOP;
    CLOSE myCursor;
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION loadWithISUMFrom(aUser recorridoTemp.usuario%TYPE) RETURNS VOID 
AS $$

DECLARE
	myCursor CURSOR FOR
		SELECT * FROM recorridoTemp
		WHERE usuario = aUser
		ORDER BY fecha_hora_ret;
	prevTupple	  recorridoTemp%ROWTYPE;
	currentTupple RECORD;
	retiroTime    TIMESTAMP;

BEGIN
	OPEN myCursor;
	FETCH myCursor INTO currentTupple;
	prevTupple := currentTupple;
	retiroTime := prevTupple.fecha_hora_ret + prevTupple.tiempo_uso;

	LOOP
		FETCH myCursor INTO currentTupple;
		EXIT WHEN NOT FOUND;
		IF(currentTupple.fecha_hora_ret > retiroTime) THEN
			INSERT INTO recorrido_final VALUES(prevTupple.periodo, aUser, prevTupple.fecha_hora_ret, 
				prevTupple.est_origen, prevTupple.est_destino, retiroTime);
			prevTupple := currentTupple;
			retiroTime := prevTupple.fecha_hora_ret + prevTupple.tiempo_uso;

		ELSE
			prevTupple.est_destino := currentTupple.est_destino;
			retiroTime := currentTupple.fecha_hora_ret + currentTupple.tiempo_uso;
		END IF;
	END LOOP;
	INSERT INTO recorrido_final VALUES(prevTupple.periodo, aUser, prevTupple.fecha_hora_ret, 
		prevTupple.est_origen, prevTupple.est_destino, retiroTime);

	CLOSE myCursor;
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION migrationProblem3() RETURNS VOID
AS $$

DECLARE
	userCursor CURSOR FOR
		SELECT DISTINCT usuario FROM recorridoTemp;
	userRecord RECORD;

BEGIN
	OPEN userCursor;

	LOOP
		FETCH userCursor INTO userRecord;
		EXIT WHEN NOT FOUND;
		PERFORM loadWithISUMFrom(userRecord.usuario);
	END LOOP;
	CLOSE userCursor;
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION detecta_solapado() RETURNS TRIGGER
AS $$
DECLARE
        triggerCursor CURSOR FOR 
                SELECT * FROM recorrido_final WHERE NEW.usuario = usuario;
        triggerRecord RECORD;
BEGIN 
        OPEN triggerCursor;
        LOOP
		FETCH triggerCursor INTO triggerRecord;
		EXIT WHEN NOT FOUND;
		IF (NOT ((NEW.fecha_hora_ret < triggerRecord.fecha_hora_ret AND NEW.fecha_hora_dev < triggerRecord.fecha_hora_ret) 
		      OR (NEW.fecha_hora_ret > triggerRecord.fecha_hora_dev AND NEW.fecha_hora_dev > triggerRecord.fecha_hora_dev)))
		THEN
		      RAISE EXCEPTION 'INSERCION IMPOSIBLE POR SOLAPAMIENTO';
		END IF;
	END LOOP;     
        CLOSE triggerCursor;
        RETURN NEW;
END;
$$ LANGUAGE PLPGSQL;


CREATE OR REPLACE FUNCTION migracion() RETURNS VOID
AS $$

BEGIN

	PERFORM migrationProblem1();
	PERFORM migrationProblem2();
	ALTER TABLE recorridoTemp ADD PRIMARY KEY(usuario,fecha_hora_ret);
	PERFORM migrationProblem3();

	DROP TABLE recorrido_importado;
	DROP TABLE recorridoTemp;

END;
$$ LANGUAGE PLPGSQL;


CREATE TRIGGER detecta_solapado
BEFORE INSERT ON recorrido_final
FOR EACH ROW
EXECUTE PROCEDURE detecta_solapado();

SELECT migracion();
SELECT * FROM recorrido_final;

