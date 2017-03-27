DROP FUNCTION check_is_absence(date, int);
 
CREATE OR REPLACE FUNCTION check_is_absence(
    IN _day date,
    IN _employee_id int,
    OUT absence character varying)
  RETURNS character varying AS

$BODY$
	
BEGIN

  EXECUTE 'select absences.absence_name
	from
		absence_journal 
	LEFT JOIN absences ON absences.id = absence_journal.absence_id
	where ''' || _day || ''' between absence_begin and absence_end
		and employee_id = ' || _employee_id INTO absence;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION check_is_absence(date, int)
  OWNER TO postgres;