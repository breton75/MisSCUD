DROP FUNCTION calc_employee_sum_precise(int, date, date);

CREATE OR REPLACE FUNCTION calc_employee_sum_precise(
	IN _employee_id int,
	IN _begin date,
	IN _end date,
	OUT total_time interval)
	
  RETURNS interval AS

$BODY$
  declare
	r1  record;
	r2  record;
	--t_r record;
	time_1 timestamp without time zone;
	time_2 timestamp without time zone;
	period_begin timestamp without time zone;
	period_final timestamp without time zone;
	scud_in int = 1;
	scud_out int = 2;
	scud_num int;
	doCalc boolean;
	--total interval;

begin

  period_begin = _begin + time '00:00:00';
  period_final = _end + time '23:59:59';
  

  total_time = 0;


/* разбираем записи парами. в иделе идет вход-выход. 
   в общем случаем берем первую запись в паре, затем, если это вход, то читаем вторую запись. если первая запись в паре - выход, то читаем предыдущую запись.
   рассматриваем все возможные варианты двух записей */
	   
  EXECUTE 'select sensor_number, date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || period_begin || ''' and date_time <= ''' || period_final || ''' ORDER BY date_time ASC limit 1' INTO r1;



  time_1 = r1.date_time;
  scud_num = r1.sensor_number;
  
  while not (time_1 is null) 
  LOOP

	doCalc = true;
	
	if scud_num = scud_in then   -- первая запись пары - вход


		/* находим следующую запись.
		возможны 5 вариантов: 
		1. следующая запись - выход в этот же день. считаем интервал между входом и выходом и переходим к следующей итерации
		2. следующая запись - выход, но в другой день. в этом случае время выхода засчитываем 18:00 этого же дня, считаем интревал между входом и выходом, переходим к следующей итерации
		3. следующая запись - вход в этот же день. в этом случае время входа определяем по второй записи. и переходим к следущей итерации. интервал между этими записями не учитываем
		4. следующая запись - вход, но в другой день. в этом случае время выхода засчитываем 18:00 этого же дня, считаем интревал между входом и выходом, переходим к следующей итерации
		5. следующей записи нет (null). в этом случае время выхода засчитываем 18:00 этого же дня, считаем интревал между входом и выходом, переходим к следующей итерации (с выходом)
		*/

		EXECUTE 'select sensor_number, date_time from scud_history 
			where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
				and date_time > ''' || time_1 || ''' and date_time <= ''' || period_final || ''' ORDER BY date_time ASC limit 1' INTO r2;

		
		/* перебираем варианты */
		/* есть ли следующая запись ? */
		if r2.date_time is null then --5.

			if time_1 > (date_trunc('day', time_1) + interval '18 hours') then -- если последняя запись в этот день больше 18:00, то считаем разницу между этой записью и 18:00.

				time_2 = time_1;
				time_1 = date_trunc('day', time_1) + interval '18 hours';
				
			else
				time_2 = date_trunc('day', time_1) + interval '18 hours'; -- конец работы в 18 часов этого же дня

			end if;

		/* тот же день, что и первая запись ? */
		elsif date_trunc('day', time_1) = date_trunc('day', r2.date_time) then  -- тот же день
			
			/* вход или выход ? */
			if r2.sensor_number = scud_in then --3.

				time_1 = r2.date_time;
				doCalc = false;

			else
				time_2 = r2.date_time; -- 1.

			end if;

		else  -- другой день
			
			/* вход или выход одинаковый алгоритм */
			time_2 = date_trunc('day', time_1) + interval '18 hours'; -- 2. 4. -- конец работы в 18 часов этого же дня


		end if;

	
	else  -- первая запись пары - выход

		/* находим предыдущую запись пары
		возможны 5 вариантов: 
		1. предыдущая запись - выход в этот же день. временем входа считаем время предущей записи. считаем интревал между входом и выходом, переходим к следующей итерации
		2. предыдущая запись - выход, но в другой день. в этом случае время входа засчитываем 09:00 этого же дня, считаем интревал между входом и выходом, переходим к следующей итерации
		3. предыдущая запись - вход в этот же день. в принципе невозможный вариант, но все же... считаем интревал между входом и выходом. переходим к следущей итерации. 
		4. предыдущая запись - вход, но в другой день. время входа засчитываем 09:00 этого же дня. считаем интревал между входом и выходом. переходим к следущей итерации. 
		5. предыдущей записи нет (null). время входа засчитываем 09:00 этого же дня. считаем интревал между входом и выходом, переходим к следующей итерации (с выходом)
		*/

		EXECUTE 'select sensor_number, date_time from scud_history 
			where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
				and date_time >= ''' || period_begin || ''' and date_time < ''' || time_1 || ''' ORDER BY date_time DESC limit 1' INTO r2;

		
		/* перебираем варианты */
		/* есть ли предыдущая запись ? */
		if r2.date_time is null then --5.

			if time_1 < (date_trunc('day', time_1) + interval '9 hours') then
			
				time_2 = date_trunc('day', time_1) + interval '9 hours';  -- время выхода

			else

				time_2 = time_1;  -- время выхода
				time_1 = date_trunc('day', time_1) + interval '9 hours'; -- временем входа считаем начало работы в 09 часов этого же дня

			end if;

		/* тот же день, что и первая запись ? */
		elsif date_trunc('day', time_1) = date_trunc('day', r2.date_time) then  -- тот же день. 1. 3. вход или выход в этот же день - одинаковый алгоритм

			time_2 = time_1;  -- время выхода
			time_1 = r2.date_time;  -- временем входа считаем время предущей записи

		else  -- другой день. 2. 4. вход или выход одинаковый алгоритм 

			time_2 = time_1;  -- время выхода
			time_1 = date_trunc('day', time_1) + interval '9 hours'; -- временем входа считаем начало работы в 09 часов этого же дня

		end if;


		

	end if;

	if doCalc = true then
		/* считаем интервал между входом и выходом */
		total_time = total_time + (time_2 - time_1);

		EXECUTE 'select sensor_number, date_time from scud_history 
			where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
				and date_time > ''' || time_2 || ''' and date_time <= ''' || period_final || ''' ORDER BY date_time ASC limit 1' INTO r1;

		time_1 = r1.date_time;
		scud_num = r1.sensor_number;
  
	end if;


  END LOOP;

  --total_time = total

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION calc_employee_sum_precise(int, date, date)
  OWNER TO postgres;



