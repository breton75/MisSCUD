DROP FUNCTION new_scudkey(int, int, text);
 
CREATE OR REPLACE FUNCTION new_scudkey(
    IN _key_value int,
    IN _employee_id int,
    IN _description text,
    OUT new_scudkey_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into scuds (key_value, employee_id, description) values(' || 
			_key_value   || ', '   || 
			_employee_id || ', ''' ||
		        _description || ''')';


  EXECUTE 'select MAX(id) from scuds' INTO new_scudkey_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_scudkey(int, int, text)
  OWNER TO postgres;