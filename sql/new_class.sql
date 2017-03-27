DROP FUNCTION new_class(character varying,
			character varying,
			bytea,
			character varying);
 
CREATE OR REPLACE FUNCTION new_class(
    IN _class_name character varying,
    IN _class_lib_path character varying,
    IN _icon bytea,
    IN _description character varying,
    OUT new_class_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into classes (class_name, class_lib_path, icon, description) values(''' || 
			_class_name     || ''', '''  || 
		        _class_lib_path || ''', '''  ||
		        _icon           || ''', '''  ||
		        _description    || ''')';


  EXECUTE 'select MAX(id) from classes' INTO new_class_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_class(character varying,
			 character varying,
			 bytea,
			 character varying)
  OWNER TO postgres;