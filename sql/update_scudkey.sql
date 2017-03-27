DROP FUNCTION update_scudkey(int, int, text, int);
 
CREATE OR REPLACE FUNCTION update_scudkey(
    IN _key_value int,
    IN _employee_id int,
    IN _description text,
    IN _id int)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update scuds set '      ||
		'key_value = '     || _key_value   || ', ' ||
		'employee_id = '   || _employee_id || ', ' ||
		'description = ''' || _description || ''''  ||
	'where id = ' || _id;


END;
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_scudkey(int, int, text, int)
  OWNER TO postgres;