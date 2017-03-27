DROP FUNCTION update_class(character varying,
			character varying,
			bytea,
			character varying,
			int);
 
CREATE OR REPLACE FUNCTION update_class(
    IN _class_name character varying,
    IN _class_lib_path character varying,
    IN _icon bytea,
    IN _description character varying,
    IN _class_id int)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'update classes set ' ||
		'class_name = '''     || _class_name     || ''', ' ||
		'class_lib_path = ''' || _class_lib_path || ''', ' ||
		'icon = '''           || _icon           || ''', ' || 
		'description = '''    || _description    || ''''   ||
		'where id = ' || _class_id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_class(character varying,
			 character varying,
			 bytea,
			 character varying,
			 int)
  OWNER TO postgres;