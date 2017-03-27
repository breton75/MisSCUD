DROP FUNCTION check_ip_exists(int);
 
CREATE OR REPLACE FUNCTION check_ip_exists(
    IN _ip int,
    OUT cnt int)
  RETURNS int AS

$BODY$
BEGIN

  EXECUTE 'select count(id) from devices where ip = ' || _ip INTO cnt;


END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION check_ip_exists(int)
  OWNER TO postgres;