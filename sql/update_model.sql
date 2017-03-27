DROP FUNCTION update_model(character varying,
			  integer,
			  integer,
			  character varying,
			  character varying,
			  integer);
 
CREATE OR REPLACE FUNCTION update_model(
    IN _model_name character varying,
    IN _class_id integer,
    IN _brand_id integer,
    IN _model_lib_path character varying,
    IN _description character varying,
    IN _id integer)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update models set '
			'model_name = '''     || _model_name     || ''', ' ||
			'class_id = '         || _class_id       || ', '   ||
			'brand_id = '         || _brand_id       || ', '   ||
			'model_lib_path = ''' || _model_lib_path || ''', ' ||
			'description = '      || _description    || '''';

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_model(character varying,
			  integer,
			  integer,
			  character varying,
			  character varying,
			  integer)
  OWNER TO postgres;