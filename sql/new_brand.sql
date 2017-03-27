DROP FUNCTION new_brand(character varying,
			character varying);
 
CREATE OR REPLACE FUNCTION new_brand(
    IN _brand_name character varying,
    IN _description character varying,
    OUT new_brand_id integer)
  RETURNS integer AS

$BODY$
BEGIN

  EXECUTE 'insert into brands (brand_name, description) values(''' || 
			_brand_name      || ''', ''' || 
		        _description     || ''''     || ')';


  EXECUTE 'select MAX(id) from brands' INTO new_brand_id ;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_brand(character varying,
			 character varying)
  OWNER TO postgres;