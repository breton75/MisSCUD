DROP FUNCTION new_holiday(character varying);
 
CREATE OR REPLACE FUNCTION new_holiday(
    IN _holiday_name character varying,
    OUT new_holiday_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into holidays (holiday_name) values(''' || 
			_holiday_name      || ''')';


  EXECUTE 'select MAX(id) from holidays' INTO new_holiday_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_holiday(character varying)
  OWNER TO postgres;