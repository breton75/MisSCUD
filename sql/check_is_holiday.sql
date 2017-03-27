DROP FUNCTION check_is_holiday(date);

CREATE OR REPLACE FUNCTION check_is_holiday(
    IN _day date,
    OUT holiday character varying)
  RETURNS character varying AS
$BODY$
declare
	dayOfWeek int;
	
BEGIN

  EXECUTE 'select holidays.holiday_name
	from
		holidays_calendar 
		LEFT JOIN holidays ON holidays.id = holidays_calendar.holiday_id
	where holiday_date = ''' || _day || '''' INTO holiday;

  if holiday is NULL then
  
	EXECUTE 'select EXTRACT(DOW FROM (''' || _day || '''::date + ''00:00:00''::time))' INTO dayOfWeek;

	if dayOfWeek = 0 then holiday = 'Воскресенье';
	elsif dayOfWeek = 6 then holiday = 'Суббота';
	end if;

  end if;

END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION check_is_holiday(date)
  OWNER TO postgres;