DROP FUNCTION calc_employee_sum_by_day_simple(int, date);

CREATE OR REPLACE FUNCTION calc_employee_sum_by_day_simple(
	IN _employee_id int,
	IN _day date,
	OUT date_time_1 timestamp without time zone,
	OUT date_time_2 timestamp without time zone,
	OUT work_time interval)

  RETURNS record AS

$BODY$
  declare
	r1  record;
	r2  record;
	day_begin timestamp without time zone;
	day_final timestamp without time zone;

begin

  /* начало и конец дня */
  day_begin = _day + time '00:00:00';
  day_final = _day + time '23:59:59';


  /* здесь не учитывается направление прохода (вход или выход). поэтому sensor_number не рассматриваем */

  /* находим первую запись в текущий день */
  EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time ASC limit 1' INTO r1;


  /* находим последнюю запись в текущий день */
  EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time DESC limit 1' INTO r2;

  date_time_1 = r1.date_time;
  date_time_2 = r2.date_time;
  
  if (r1.date_time is not NULL) and (r2.date_time is not NULL) and (r1.date_time <> r2.date_time) then

	/* считаем интервал между первой и второй записью */
	work_time = date_time_2 - date_time_1;
	
  else

	work_time = NULL;
  
  end if;

  if date_time_1 = date_time_2 then date_time_2 = NULL; end if;


end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION calc_employee_sum_by_day_simple(int, date)
  OWNER TO postgres;



