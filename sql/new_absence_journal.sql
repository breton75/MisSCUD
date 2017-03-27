DROP FUNCTION new_absence_journal(int, int, date, date);
 
CREATE OR REPLACE FUNCTION new_absence_journal(
    IN _employee_id int,
    IN _absence_id int,
    IN _begin date,
    IN _end date,
    OUT new_absence_journal_id int)
  RETURNS int AS

$BODY$
BEGIN

  EXECUTE 'insert into absence_journal (employee_id, absence_id, absence_begin, absence_end) values(' ||
				_employee_id || ', '     || 
				_absence_id  || ', '''   ||
				_begin       || ''', ''' ||
				_end         || ''')';

  EXECUTE 'select MAX(id) from absence_journal' INTO new_absence_journal_id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_absence_journal(int, int, date, date)
  OWNER TO postgres;