--DROP FUNCTION calc_employee_sum_first_in_last_out(int, date, date);

CREATE OR REPLACE FUNCTION calc_employee_sum_first_in_last_out(
	IN _employee_id int,
	IN _begin date,
	IN _end date,
	OUT total_time interval)
	
  RETURNS interval AS

$BODY$

/* **************************************************************************************************************
  ФУНКЦИЯ ВОЗВРАЩАЕТ ОБЩЕЕ ВРЕМЯ ОТРАБОТАННОЕ СОТРУДНИКОМ ЗА УКАЗАННЫЙ ПЕРИОД

  ВОЗВРАЩАЕМЫЕ ЗНАЧЕНИЯ:
  total_time - общее время отработанное за период, как сумма отработанного времени за каждый день периода.

  ВРЕМЯ ЗА ДЕНЬ СЧИТАЕТСЯ КАК РАЗНИЦА МЕЖДУ ПЕРВЫМ ВХОДОМ И ПОСЛЕДНИМ ВЫХОДОМ

*************************************************************************************************************** */
   
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
	
	/* находим первый вход в текущий день */
	EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and sensor_number = 1
				and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time ASC limit 1' INTO r1;

			
	/* находим последний выход в текущий день */
	EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and sensor_number = 2
				and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || ''' ORDER BY date_time DESC limit 1' INTO r2;

	/* считаем интервал между ними.
	если первый вход или последний выход равны NULL, то интревал не считаем. рабочее время в этот день будет равно 0 */
	if (r1.date_time is not NULL) and (r2.date_time is not NULL) then
	 
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



