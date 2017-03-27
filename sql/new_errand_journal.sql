DROP FUNCTION new_errand_journal(int, timestamp without time zone, timestamp without time zone, character varying, character varying);
 
CREATE OR REPLACE FUNCTION new_errand_journal(
    IN _employee_id int,
    IN _errand_begin timestamp,
    IN _errand_end timestamp,
    IN _destination character varying,
    IN _description character varying,
    OUT new_errand_journal_id int)
  RETURNS int AS

$BODY$
BEGIN

  EXECUTE 'insert into errand_journal (employee_id, errand_begin, errand_end, destination, description) values(' ||
				_employee_id  || ', '''   || 
				_errand_begin || ''', ''' ||
				_errand_end   || ''', ''' ||
				_destination  || ''', ''' ||
				_description  || ''')';
				 -- || _errand_date  || ''')';

  EXECUTE 'select MAX(id) from errand_journal' INTO new_errand_journal_id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_errand_journal(int, timestamp, timestamp, character varying, character varying)
  OWNER TO postgres;