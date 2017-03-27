DROP FUNCTION update_department(character varying,
			int,
			bytea,
			character varying,
			int);
 
CREATE OR REPLACE FUNCTION update_department(
    IN _department_name character varying,
    IN _boss_id int,
    IN _icon bytea,
    IN _description character varying,
    IN _department_id int)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update departments set ' ||
		'department_name = ''' || _department_name || ''', ' ||
		'boss_id = '           || _boss_id         || ', '   ||
		'icon = '''            || _icon            || ''', ' || 
		'description = '''     || _description     || ''''   ||
		'where id = ' || _department_id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_department(character varying,
			 int,
			 bytea,
			 character varying,
			 int)
  OWNER TO postgres;