DROP FUNCTION update_holidays_calendar(date, int);
 
CREATE OR REPLACE FUNCTION update_holidays_calendar(
    IN _date date,
    IN _holiday_id int)
  RETURNS void AS

$BODY$
declare
	cnt int = 0;

BEGIN

  EXECUTE 'select count(holiday_date) from holidays_calendar where holiday_date = ''' || _date || '''' INTO cnt;
  
  if (cnt = 0) and (_holiday_id <> -1) then
  
	EXECUTE 'insert into holidays_calendar (holiday_date, holiday_id) values(''' ||
				_date       || ''', ' || 
				_holiday_id || ')';

  elsif (cnt > 0) and (_holiday_id <> -1) then

	EXECUTE 'update holidays_calendar set holiday_id = ' || _holiday_id || ' where holiday_date = ''' || _date || '''';

  elsif _holiday_id = -1 then

	EXECUTE 'delete from holidays_calendar where holiday_date = ''' || _date || '''';

  end if;
  

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION update_holidays_calendar(date, int)
  OWNER TO postgres;