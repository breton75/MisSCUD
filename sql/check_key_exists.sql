DROP FUNCTION check_key_exists(int);
 
CREATE OR REPLACE FUNCTION check_key_exists(
    IN _key_value int,
    OUT key_count integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'select count(id) from scuds where key_value = ' || _key_value INTO key_count;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION check_key_exists(int)
  OWNER TO postgres;