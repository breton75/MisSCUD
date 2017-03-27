DROP FUNCTION update_device(character varying,
			  int,
			  int,
			  int,
			  integer,
			  integer,
			  --bytea,
			  text,
			  integer);
 
CREATE OR REPLACE FUNCTION update_device(
    IN _device_name character varying,
    IN _model_id int,
    IN _class_id int,
    IN _brand_id int,
    IN _ip integer,
    IN _port integer,
    --IN _config bytea,
    IN _description text,
    IN _id integer)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update devices set '        ||
		'device_name = '''     || _device_name     || ''', '   ||
		'model_id = '          || _model_id        || ', '     || 
		'class_id = '          || _class_id        || ', '     ||
		'brand_id = '          || _brand_id        || ', '     ||
		'ip = '                || _ip              || ', '     ||
		'port = '              || _port            || ', '     ||
		--'device_config = '''   || _config	   || ''', '   ||
		'description = '''     || _description     || ''''     ||
	'where id = ' || _id;


END;
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_device(character varying,
			  int,
			  int,
			  int,
			  integer,
			  integer,
			  --bytea,
			  text,
			  integer)
  OWNER TO postgres;