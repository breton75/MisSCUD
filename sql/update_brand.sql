DROP FUNCTION update_brand(character varying,
			    text,
			    integer);
 
CREATE OR REPLACE FUNCTION update_brand(
    IN _brand_name character varying,
    IN _description text,
    IN _id integer)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update brands set '        ||
		'brand_name = '''     || _brand_name     || ''', ' ||
		'description = '''    || _description     || ''''  ||
	'where id = ' || _id;


END;
$BODY$
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_brand(character varying,
			     text,
			     integer)
  OWNER TO postgres;