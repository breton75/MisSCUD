DROP FUNCTION calc_employee_sum_simple(int, date, date);

CREATE OR REPLACE FUNCTION calc_employee_sum_simple(
	IN _employee_id int,
	IN _begin date,
	IN _end date,
	OUT total_time interval)
	
  RETURNS interval AS

$BODY$
  declare
	r1  record;
	r2  record;

	day_begin timestamp without time zone;
	day_final timestamp without time zone;

	period_begin timestamp without time zone;
	period_final timestamp without time zone;


begin

  period_begin = _begin + time '00:00:00';
  period_final = _end + time '23:59:59';

  total_time = 0;

  /* определяем первый день периода */
  day_begin = period_begin;
  day_final = date_trunc('day', day_begin) + interval '23:59:59';

  /* перебираем все дни периода */
  while day_final <= period_final
  LOOP
	/* здесь не учитывается направление прохода (вход или выход). поэтому sensor_number не рассматриваем */
	
	/* находим первый проход в текущий день */
	EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time ASC limit 1' INTO r1;

			
	/* находим последний проход в текущий день */
	EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time DESC limit 1' INTO r2;

	/* считаем интервал между ними.
	1) если время первого прохода = времени второго, то интервал не считаем. 
	2) если первый или второй проходы равны NULL, то интревал не считаем */
	if (r1.date_time is not NULL) and (r2.date_time is not NULL) and (r1.date_time <> r2.date_time) then
	 
		total_time = total_time + (r2.date_time - r1.date_time);

	end if;

	/* определяем следующий день */
	day_begin = day_begin + interval '24 hours';
	day_final = day_final + interval '24 hours';	

  END LOOP;

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION calc_employee_sum_simple(int, date, date)
  OWNER TO postgres;



