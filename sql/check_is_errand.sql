DROP FUNCTION check_is_errand(date, int);
 
CREATE OR REPLACE FUNCTION check_is_errand(
    IN _day date,
    IN _employee_id int,
    OUT errand_duration interval)
  RETURNS interval AS

$BODY$
declare
	r record;
	t1 timestamp without time zone;
	t2 timestamp without time zone;
	
	
BEGIN

  t1 = _day + time '00:00:00';
  t2 = _day + time '23:59:59';
  errand_duration = NULL;
  
  FOR r IN EXECUTE 'select errand_begin, errand_end 
	from
		errand_journal 
	where errand_begin between ''' || t1 ''' and ''' || t2 || ''' and errand_end between ''' || t1 || ''' and ''' || t2 || '''
		and employee_id = ' || _employee_id
  LOOP

	if errand_duration = NULL then errand_duration = r.errand_end - r.errand_begin;
	else errand_duration = errand_duration::interval + (r.errand_end - r.errand_begin); end if;

  END LOOP;

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION check_is_errand(date, int)
  OWNER TO postgres;