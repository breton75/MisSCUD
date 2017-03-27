DROP FUNCTION get_user_info(character varying, character varying);
 
CREATE OR REPLACE FUNCTION get_user_info(
    IN _login character varying,
    IN _pass character varying,
    OUT user_id int,
    OUT employee_id int,
    OUT user_rights bytea)
  RETURNS record AS

$BODY$
declare
	r record;
BEGIN

  EXECUTE 'select id, employee_id, rights from users where login = ''' || _login || ''' and pass = ''' || _pass || '''' INTO r;

  user_id = r.id;
  employee_id = r.employee_id;
  user_rights = r.rights;
  
END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION get_user_info(character varying,
			 character varying)
  OWNER TO postgres;