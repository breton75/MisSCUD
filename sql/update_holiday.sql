DROP FUNCTION update_holiday(character varying,
			    integer);
 
CREATE OR REPLACE FUNCTION update_holiday(
    IN _holiday_name character varying,
    IN _id integer)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update holidays set '        ||
		'holidays_name = '''     || _holiday_name     || '''' ||
	'where id = ' || _id;


END;
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_holiday(character varying,
			    integer)
  OWNER TO postgres;