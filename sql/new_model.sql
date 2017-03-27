DROP FUNCTION new_model(character varying,
			  integer,
			  integer,
			  character varying,
			  character varying);
 
CREATE OR REPLACE FUNCTION new_model(
    IN _model_name character varying,
    IN _class_id integer,
    IN _brand_id integer,
    IN _model_lib_path character varying,
    IN _description character varying,
    OUT new_model_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into models (model_name, class_id, brand_id, model_lib_path, description) values(''' || 
			_model_name     || ''', '   || 
		        _class_id       || ', '     ||
		        _brand_id       || ', '''   ||
		        _model_lib_path || ''', ''' ||
		        _description    || ''')';


  EXECUTE 'select MAX(id) from models' INTO new_model_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_model(character varying,
			  integer,
			  integer,
			  character varying,
			  character varying)
  OWNER TO postgres;