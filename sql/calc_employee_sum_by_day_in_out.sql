DROP FUNCTION calc_employee_sum_by_day_in_out(date, int);

/* ******************************************************************************** */
/* *********   РАСЧЕТ КОЛИЧЕСТВА ОТРАБОТАННЫХ СОТРУДНИКОМ ЧАСОВ ЗА 1 ДЕНЬ  ******** */
/* *********   С УЧЕТОМ МЕСТНЫХ КОМАНДИРОВОК.                              ******** */
/* *********   МЕТОД РАСЧЕТА: ПЕРВЫЙ ВХОД - ПОСЛЕДНИЙ ВЫХОД  ********************** */
/* ******************************************************************************** */

CREATE OR REPLACE FUNCTION calc_employee_sum_by_day_in_out(
	IN _day date,
	IN _employee_id int,
	OUT date_time_1 timestamp without time zone,
	OUT date_time_2 timestamp without time zone,
	OUT work_time interval)

  RETURNS record AS

$BODY$
  declare
	r1  record;
	day_begin timestamp without time zone;
	day_final timestamp without time zone;

begin

  /* начало и конец дня */
  day_begin = _day + time '00:00:00';
  day_final = _day + time '23:59:59';

  /* находим первый вход в текущий день */
  EXECUTE 'select date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || '''
				and sensor_number = 1 ORDER BY date_time ASC limit 1' INTO date_time_1;


  /* находим последний выход в текущий день */
  EXECUTE 'select date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || '''
				and sensor_number = 2 ORDER BY date_time DESC limit 1' INTO date_time_2;

  --date_time_1 = r1.date_time;
  --date_time_2 = r2.date_time;
  
  if (date_time_1 is not NULL) and (date_time_2 is not NULL) and (date_time_1 < date_time_2) then

	/* считаем интервал между первой и второй записью */
	work_time = date_time_2 - date_time_1;
	
  else

	work_time = NULL;
  
  end if;

  /* определяем время в местных командировках на текущий день */
  errand_time = NULL;

  /* местные командировки осуществляются только в течение одного дня, 
     поэтому достаточно определить начало командировки, чтобы понять, 
     что эта командировка относится к текущему дню */

  FOR r1 IN EXECUTE 'select id, errand_begin, errand_end from errand_journal 
			where employee_id = ' || _employee_id || 
				' and errand_begin between ''' || day_begin || ''' and ''' || day_final || ''''
  LOOP

	/* проверяем, что период командировки не пересекается с началом и концом работы */
	/* если пересекается, то выходим со значением -1 час */
	if (work_time is not NULL) 
		and ((r1.errand_begin between date_time_1 and date_time_2) or (r1.errand_end between date_time_1 and date_time_2)) then

		errand_time = '-1 hour'::interval;
		exit;
	end if;

	if (r1.errand_begin is not NULL) and (r1.errand_end is not NULL) and (r1.errand_begin < r1.errand_end) then
		--	errand_time = r1.errand_end - r1.errand_begin;
		--exit;
		/* считаем интервал между началом и концом командировки */
		if errand_time is NULL then
			errand_time = r1.errand_end - r1.errand_begin;
		else
			errand_time = errand_time + (r1.errand_end - r1.errand_begin);
		end if;

	end if;

  END LOOP;


end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION calc_employee_sum_by_day_in_out(int, date)
  OWNER TO postgres;



