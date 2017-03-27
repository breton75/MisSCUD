DROP FUNCTION new_department(character varying,
			int,
			bytea,
			character varying);
 
CREATE OR REPLACE FUNCTION new_department(
    IN _department_name character varying,
    IN _boss_id int,
    IN _icon bytea,
    IN _description character varying,
    OUT new_department_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into departments (department_name, boss_id, icon, description) values(''' || 
			_department_name || ''', '   || 
		        _boss_id         || ', '''   ||
		        _icon		 || ''', ''' ||
		        _description     || ''')';


  EXECUTE 'select MAX(id) from departments' INTO new_department_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_department(character varying,
			 int,
			 bytea,
			 character varying)
  OWNER TO postgres;