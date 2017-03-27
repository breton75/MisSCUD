DROP FUNCTION update_absence_journal(int, int, date, date, int);
 
CREATE OR REPLACE FUNCTION update_absence_journal(
    IN _employee_id int,
    IN _absence_id int,
    IN _begin date,
    IN _end date,
    IN _id int)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'update absence_journal set ' ||
				'employee_id = '     || _employee_id || ', '   ||
				'absence_id = '      || _absence_id  || ', '   || 
				'absence_begin = ''' || _begin       || ''', ' ||
				'absence_end = '''   || _end         || '''  ' ||
				'where id = '        || _id;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_absence_journal(int, int, date, date, int)
  OWNER TO postgres;