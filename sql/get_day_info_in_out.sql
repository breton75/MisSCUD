
DROP FUNCTION get_day_info_in_out(date, int);

CREATE OR REPLACE FUNCTION get_day_info_in_out(
    IN _day date,
    IN _employee_id int,
    OUT time_in timestamp without time zone,
    OUT time_out timestamp without time zone,
    OUT work_time interval,
    OUT dayOfWeek int,
    OUT dayOfWeek_designation character varying,
    OUT isDayOff boolean,
    OUT holiday character varying,
    OUT absence character varying,
    OUT absence_designation character varying,
    OUT errand_time interval)
  RETURNS record AS
$BODY$

/* ************************************************************************************************************************************** 
  ФУНКЦИЯ ВОЗВРАЩАЕТ ИНФОРМАЦИЮ, ОТНОСЯЩУЮСЯ К ЗАДАННОМУ ДНЮ И ПОЛЬЗОВАТЕЛЮ  								  
    																	  
    ВОЗВРАЩАЮТСЯ СЛЕДУЮЩИЕ ДАННЫЕ:
    time_in               - время первого входа;
    time_out              - время последнего выхода;
    work_time             - рабочее время за день, как разница между временем первого входа и временем последнего выхода;
    dayOfWeek             - номер дня недели;
    dayOfWeek_designation - имя дня недели;
    isDayOff              - является ли день выходным;
    holiday               - если в этот день есть праздник, то его название, иначе NULL;
    absence               - если в этот день у сотрудника есть причина для отстутствия на работе, то ее описание, иначе NULL;
    absence_designation   - условное обозначение для причины отстутствия. если ее нет, то '';
    errand_time           - если в этот день были местные командировки, то общее время местных командировок в этот день, иначе NULL 

    ВРЕМЯ ЗА ДЕНЬ СЧИТАЕТСЯ КАК РАЗНИЦА МЕЖДУ ПЕРВЫМ ВХОДОМ И ПОСЛЕДНИМ ВЫХОДОМ
    
******************************************************************************************************************************************/

declare
	r1  record;
	day_begin timestamp without time zone;
	day_final timestamp without time zone;

	weekdays character varying array[7] = ARRAY['Вск','Пнд','Втр','Срд','Чтв','Птн','Сбт'];
	
BEGIN

  holiday = NULL;
  absence = NULL;
  absence_designation = '';
  errand_time = NULL;

  day_begin = _day + time '00:00:00';
  day_final = _day + time '23:59:59';

  /* находим первый вход в текущий день */
  EXECUTE 'select date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || '''
				and sensor_number = 1 ORDER BY date_time ASC limit 1' INTO time_in;


  /* находим последний выход в текущий день */
  EXECUTE 'select date_time from scud_history 
		where key_value in (select key_value from scuds where employee_id = ' || _employee_id || ')
			and date_time >= ''' || day_begin || ''' and date_time <= ''' || day_final || '''
				and sensor_number = 2 ORDER BY date_time DESC limit 1' INTO time_out;
  
  if (time_in is not NULL) and (time_out is not NULL) and (time_in < time_out) then

	/* считаем интервал между первой и второй записью */
	work_time = time_out - time_in;
	
  else

	work_time = NULL;
  
  end if;
  
  /* день недели */
  EXECUTE 'select EXTRACT(DOW FROM ''' || day_begin || '''::timestamp)' INTO dayOfWeek;
  dayOfWeek_designation = weekdays[dayOfWeek + 1];

  /* определяем, праздничный ли день */
  EXECUTE 'select holidays.holiday_name
	from
		holidays_calendar 
		LEFT JOIN holidays ON holidays.id = holidays_calendar.holiday_id
	where holiday_date = ''' || _day || '''' INTO holiday;

  /* если день не праздничный, то определяем не выходной ли. назначаем значение не NULL */
  isDayOff = holiday is not NULL or dayOfWeek = 0 or dayOfWeek = 6;


  /* была ли в этот день причина для отсутствия на работе */
  EXECUTE 'select absences.absence_name, absences.designation
	from
		absence_journal 
	LEFT JOIN absences ON absences.id = absence_journal.absence_id
	where ''' || _day || ''' between absence_begin and absence_end
		and employee_id = ' || _employee_id INTO absence, absence_designation;


  /* были ли в этот день местные командировки */

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
		and ((r1.errand_begin between time_in and time_out) or (r1.errand_end between time_in and time_out)) then

		errand_time = '-1 hour'::interval;
		exit;
	end if;

	if r1.errand_begin is not NULL and r1.errand_end is not NULL then

		/* считаем интервал между началом и концом командировки */
		if errand_time is NULL then errand_time = r1.errand_end - r1.errand_begin;
		else errand_time = errand_time + (r1.errand_end - r1.errand_begin);
		end if;

	end if;

  END LOOP;

  

END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION get_day_info_in_out(date, int)
  OWNER TO postgres;