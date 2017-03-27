DROP FUNCTION update_errand_journal(int, timestamp without time zone, timestamp without time zone, character varying, character varying, int);
 
CREATE OR REPLACE FUNCTION update_errand_journal(
    IN _employee_id int,
    IN _errand_begin timestamp without time zone,
    IN _errand_end timestamp without time zone,
    IN _destination character varying,
    IN _description character varying,
    IN _id int)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update errand_journal set employee_id = ' || _employee_id || ', 
					errand_begin = ''' || _errand_begin || ''', 
					errand_end = ''' || _errand_end || ''', 
					destination = ''' || _destination || ''', 
					description = ''' || _description || '''
				where id = ' || _id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_errand_journal(int, timestamp without time zone, timestamp without time zone, character varying, character varying, int)
  OWNER TO postgres;