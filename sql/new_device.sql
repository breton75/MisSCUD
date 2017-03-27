 DROP FUNCTION new_device(character varying,
			  integer,
			  integer,
			  integer,
			  integer,
			  integer,
			  --bytea,
			  text);
 
CREATE OR REPLACE FUNCTION new_device(
    IN _device_name character varying,
    IN _model_id integer,
    IN _class_id integer,
    IN _brand_id integer,
    IN _ip integer,
    IN _port integer,
    --IN _config bytea,
    IN _description text,
    OUT new_device_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into devices (device_name, model_id, class_id, brand_id, ip, port, description) values(''' || 
			_device_name     || ''', '   || 
			_model_id        || ', '     || 
		        _class_id        || ', '     ||
		        _brand_id        || ', '     ||
		        _ip              || ', '     ||
		        _port            || ', '''   ||
		        --_config		 || ''', ''' ||
		        _description     || ''''     || ')';


  EXECUTE 'select MAX(id) from devices' INTO new_device_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_device(character varying,
			  integer,
			  integer,
			  integer,
			  integer,
			  integer,
			  --bytea,
			  text)
  OWNER TO postgres;