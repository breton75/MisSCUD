
  /* ****************************************************** */
  /* *******  ОТЧЕТ. ТАБЕЛЬ УЧЕТА РАБОЧЕГО ВРЕМЕНИ  ******* */
  /* ****************************************************** */

DROP FUNCTION report_timesheet(int, int, int, int);

CREATE OR REPLACE FUNCTION report_timesheet(
	IN _employee_id int,
	IN _department_id int,
	IN _month int,
	IN _year int)
	
  RETURNS TABLE(duty character varying,
		id int,
		lvl int,
		bgnd_color int,
		font_color int,
		font_bold boolean,
		font_italic boolean,

		numpp character varying,
		employee character varying,
		employee_position character varying,

		field1 character varying,
		field2 character varying,
		field3 character varying,
		field4 character varying,
		field5 character varying,
		field6 character varying,
		field7 character varying,
		field8 character varying,
		field9 character varying,
		field10 character varying,
		field11 character varying,
		field12 character varying,
		field13 character varying,
		field14 character varying,
		field15 character varying,
		field16 character varying,
		field17 character varying,
		field18 character varying,
		field19 character varying,
		field20 character varying,
		field21 character varying,
		field22 character varying,
		field23 character varying,
		field24 character varying,
		field25 character varying,
		field26 character varying,
		field27 character varying,
		field28 character varying,
		field29 character varying,
		field30 character varying,
		field31 character varying,
		total_days character varying,
		total_hours character varying) AS

$BODY$
  declare
	d_r record;
	e_r record;
	t_r record;
	p_r record;
	day_r record;
	condition character varying;

	daysInMonth int;
	dayOfWeek int;

	isHoliday boolean;
	isAbsence boolean;
	absence_designation character varying;

	cur_day int;
	fields character varying array[31];

	full_days_cnt int;
	full_hour_cnt int;
	days_cnt int;
	hour_cnt int;
	
	tmp int;
	tmp2 int;
	num int;
	
	period_begin timestamp without time zone;
	period_final timestamp without time zone;
	day_1 date;

	months character varying array[12] = array['Январь','Февраль','Март','Апрель','Май','Июнь','Июль','Август','Сентябрь','Октябрь','Ноябрь','Декабрь'];

begin

  set datestyle to german;

  period_begin = ('01/' || _month || '/' || _year)::date + time '00:00:00' ;
  EXECUTE 'SELECT EXTRACT(DAY FROM (date_trunc(''month'', ''' || period_begin || '''::date + interval ''1 month'') - date_trunc(''month'', ''' || period_begin || '''::date)))' INTO daysInMonth;
  period_final = (daysInMonth || '/' || _month || '/' || _year)::date + time '23:59:59';

  /* HEADER */
  duty = NULL; --'-<duty';
  id = NULL; --'-<id';
  lvl = NULL;
  bgnd_color = NULL;
  font_color = NULL; 
  font_bold = NULL;
  font_italic = NULL;

  numpp = '+|';
  employee = '+<';
  employee_position = '+<';

  field1  = '+|';
  field2  = '+|';
  field3  = '+|';
  field4  = '+|';
  field5  = '+|';
  field6  = '+|';
  field7  = '+|';
  field8  = '+|';
  field9  = '+|';
  field10 = '+|';
  field11 = '+|';
  field12 = '+|';
  field13 = '+|';
  field14 = '+|';
  field15 = '+|';
  field16 = '+|';
  field17 = '+|';
  field18 = '+|';
  field19 = '+|';
  field20 = '+|';
  field21 = '+|';
  field22 = '+|';
  field23 = '+|';
  field24 = '+|';
  field25 = '+|';
  field26 = '+|';
  field27 = '+|';
  field28 = '+|';
  
  if daysInMonth > 28 then field29 = '+|';
  else field29 = '-|'; end if;
  
  if daysInMonth > 29 then field30 = '+|';
  else field30 = '-|'; end if;
  
  if daysInMonth > 30 then field31 = '+|';
  else field31 = '-|'; end if;
  
  total_days = '+|';
  total_hours = '+|';
  
  RETURN NEXT;

  /* ШАПКА ТАБЕЛЯ */
  bgnd_color = x'FFFFFF'::int;
  font_color = 0; 
  font_bold = true;
  font_italic = false;

  numpp = NULL;
  employee = NULL;
  employee_position = NULL;
  total_hours = NULL;
  total_days = NULL;

  field1  = NULL;
  field2  = NULL;
  field3  = NULL;
  field4  = NULL;
  field5  = NULL;
  field6  = NULL;
  field7  = NULL;
  field8  = NULL;
  field9  = NULL;
  field10 = NULL;
  field11 = NULL;
  field12 = NULL;
  field13 = NULL;
  field14 = NULL;
  field15 = NULL;
  field16 = NULL;
  field17 = NULL;
  field18 = NULL;
  field19 = NULL;
  field20 = NULL;
  field21 = NULL;
  field22 = NULL;
  field23 = NULL;
  field24 = NULL;
  field25 = NULL;
  field26 = NULL;
  field27 = NULL;
  field28 = NULL;
  field29 = NULL;
  field30 = NULL;
  field31 = NULL;
  
  employee = 'ТАБЕЛЬ УЧЕТА РАБОЧЕГО ВРЕМЕНИ';
  RETURN NEXT;

  employee = 'Период: ' || months[extract(month from period_begin)] || ' ' || extract(year from period_begin); --   period_begin::date || ' - ' || period_final::date;
  RETURN NEXT;
  
  /** определяем количество выходных и рабочих дней и часов **/
  tmp =  extract(dow from period_begin);
  tmp2 = extract(day from period_final);

  /*** количество выходных **/
  EXECUTE 'select count(id) from holidays_calendar where holiday_date between ''' || period_begin || '''::date and ''' || period_final || '''::date' into full_days_cnt;
  full_days_cnt = tmp2 - (full_days_cnt + (tmp2/7) * 2 + div(tmp2%7 + tmp, 6) + div(tmp2%7 + tmp, 7));
  full_hour_cnt = full_days_cnt * 8;
  
  employee = 'Рабочих дней/часов: ' ||  full_days_cnt || ' / ' || full_hour_cnt;
  RETURN NEXT;

  employee = '{style: fgnd_color: CD4343}Выходных дней: ' || tmp2 - full_days_cnt;
  RETURN NEXT;
  
  
  if(_employee_id = -1) and (_department_id = -1) then condition = '';
  elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
  else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
  end if;
  
  FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
  LOOP
	/* вставляем пустую строку */
	id = NULL;
	duty = NULL;
	lvl = 0;
	bgnd_color = x'FFFFFF'::int;
	font_color = 0; 
	font_bold = false;
	font_italic = false;
	
	numpp = NULL;
	employee = NULL;
	employee_position = NULL;
	total_hours = NULL;
	total_days = NULL;
	
	field1  = NULL;
	field2  = NULL;
	field3  = NULL;
	field4  = NULL;
	field5  = NULL;
	field6  = NULL;
	field7  = NULL;
	field8  = NULL;
	field9  = NULL;
	field10 = NULL;
	field11 = NULL;
	field12 = NULL;
	field13 = NULL;
	field14 = NULL;
	field15 = NULL;
	field16 = NULL;
	field17 = NULL;
	field18 = NULL;
	field19 = NULL;
	field20 = NULL;
	field21 = NULL;
	field22 = NULL;
	field23 = NULL;
	field24 = NULL;
	field25 = NULL;
	field26 = NULL;
	field27 = NULL;
	field28 = NULL;
	field29 = NULL;
	field30 = NULL;
	field31 = NULL;

	RETURN NEXT;

	if(_employee_id = -1) and (_department_id = -1) then condition = ' where employees.deleted = false and department_id = ' || d_r.department_id;
	elsif _department_id != -1 then condition = ' where employees.deleted = false and employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
	else condition = ' where employees.deleted = false and employees.id = ' || _employee_id;
	end if;

	/* наименование отдела */
	id = d_r.department_id;
	duty = 'department';
	font_color = x'0000FF'::int;
	font_bold = true;
	employee = d_r.department_name;
	employee_position = NULL;
	
	RETURN NEXT;


	/* формируем шапку таблицы */
	id = NULL;
	duty = NULL;
	lvl = 1;
	bgnd_color = x'DCDCDC'::int;
	font_color = 0; 
	font_bold = true;
	font_italic = false;
	
	numpp = '№ пп';
	employee = 'ФИО';
	employee_position = 'Должность';

	field1  = '1';
	field2  = '2';
	field3  = '3';
	field4  = '4';
	field5  = '5';
	field6  = '6';
	field7  = '7';
	field8  = '8';
	field9  = '9';
	field10 = '10';
	field11 = '11';
	field12 = '12';
	field13 = '13';
	field14 = '14';
	field15 = '15';
	field16 = '16';
	field17 = '17';
	field18 = '18';
	field19 = '19';
	field20 = '20';
	field21 = '21';
	field22 = '22';
	field23 = '23';
	field24 = '24';
	field25 = '25';
	field26 = '26';
	field27 = '27';
	field28 = '28';
	field29 = '29';
	field30 = '30';
	field31 = '31';

	total_days = 'Всего дней';
	total_hours = 'Всего часов';

	RETURN NEXT;

	num = 1;
	
	FOR e_r IN EXECUTE 'SELECT ' ||
			'	employees.id as employee_id, ' ||
			'	concat(employees.employee_surename, '' '', ' ||
			'		substr(employees.employee_name, 1, 1), ''. '', ' ||
			'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
			'	employees.employee_position ' ||
			'FROM employees ' ||
			condition ||	
			'ORDER BY short_name ASC'
	LOOP

		id = e_r.employee_id;
		lvl = 1;
		duty = 'employee';
		bgnd_color = x'FFFFFF'::int;
		font_color = 0; 
		font_bold = false;
		font_italic = false;

		employee = e_r.short_name;
		employee_position = e_r.employee_position;

		numpp = num;
		num = num + 1;
		
		/* начинаем вычислять отработанное время на каждый день */
		day_1 = period_begin::date;
		cur_day = 1;
		days_cnt = 0;
		hour_cnt = 0;
			
		WHILE day_1 <= period_final::date
		LOOP
			--EXECUTE 'select date_time_1, date_time_2, work_time, errand_time from calc_employee_sum_by_day_in_out(' || e_r.employee_id || ', ''' || day_1 || ''')' INTO day_r;
			/* получаем информацию по дню */
			EXECUTE 'select * from get_day_info_in_out(''' || day_1 || '''::date, ' || e_r.employee_id || ')' INTO day_r;

			/* если человек НЕ работал в этот день и НЕ был в местной командировке, то определяем причину: 1) выходной или праздничный день; 2) есть запись в таблице отстутствия или 3) невыясненная причина */
			if day_r.work_time is NULL and day_r.errand_time is NULL then 

				EXECUTE 'select EXTRACT(DOW FROM (''' || day_1 || '''::date + ''00:00:00''::time))' INTO dayOfWeek;
				EXECUTE 'select count(id) > 0 as b from holidays_calendar where holiday_date = ''' || day_1 || '''' INTO isHoliday;
				
				if day_r.isDayOff then
					fields[cur_day] = '{style: fgnd_color:CD4343}В';

				else
					if (day_r.absence_designation is NULL) and (day_1 >= current_date) then fields[cur_day] = '';
					elsif day_r.absence_designation is NULL then fields[cur_day] = '{style: fgnd_color:505050}НН';
					else fields[cur_day] = day_r.absence_designation; end if;

				end if;

			/* если человек был только на работе и НЕ был в местной командировке*/
			elsif day_r.work_time is not NULL and day_r.errand_time is NULL then 

				tmp = extract (hour from day_r.work_time);
				if extract (minute from day_r.work_time) > 30 then tmp = tmp + 1; end if; -- округляем количество часов
				if day_r.time_in::time > '09:00:00'::time then
					fields[cur_day] = '{style: bgnd_color: F5B1FF}' || tmp;
				else
					fields[cur_day] = tmp;
				end if;
					
				hour_cnt = hour_cnt + tmp;
				days_cnt = days_cnt + 1;

			/* если человек был только в местной командировке и не был на работе */
			elsif day_r.work_time is NULL and day_r.errand_time is not NULL then 

				tmp = extract (hour from day_r.errand_time);
				if extract (minute from day_r.errand_time) > 30 then tmp = tmp + 1; end if; -- округляем количество часов
				
				fields[cur_day] = '{style: bgnd_color: BAFFB1 }(' || tmp || ')'; --BBFF80
									
				hour_cnt = hour_cnt + tmp;
				days_cnt = days_cnt + 1;
			
			/* если человек был и командировке и на работе */
			elsif day_r.work_time is not NULL and day_r.errand_time is not NULL then 

				tmp  = extract (hour from day_r.work_time);
				if extract (minute from day_r.work_time) > 30 then tmp = tmp + 1; end if; -- округляем количество часов
				
				/* если имеем значение -1 час, значит время командировки пересекается с временем на работе. должен разобраться человек */
				if day_r.errand_time = '-1 hour'::interval then
					fields[cur_day] = '{style: bgnd_color: BBFF80; fgnd_color: FF0000; font_bold: true}' || tmp || '(!)';
					tmp2 = 0;
				else
					tmp2 = extract (hour from day_r.errand_time);
					if extract (minute from day_r.errand_time) > 30 then tmp2 = tmp2 + 1; end if; -- округляем количество часов
					
					fields[cur_day] = '{style: bgnd_color: BBFF80}' || tmp + tmp2 || '(' || tmp2 || ')';
				end if;				
									
				hour_cnt = hour_cnt + tmp + tmp2;
				days_cnt = days_cnt + 1;
			
			end if;

			day_1 = day_1 + 1;
			cur_day = cur_day + 1;

		END LOOP;

		field1  = fields[1];
		field2  = fields[2];
		field3  = fields[3];
		field4  = fields[4];
		field5  = fields[5];
		field6  = fields[6];
		field7  = fields[7];
		field8  = fields[8];
		field9  = fields[9];
		field10 = fields[10];
		field11 = fields[11];
		field12 = fields[12];
		field13 = fields[13];
		field14 = fields[14];
		field15 = fields[15];
		field16 = fields[16];
		field17 = fields[17];
		field18 = fields[18];
		field19 = fields[19];
		field20 = fields[20];
		field21 = fields[21];
		field22 = fields[22];
		field23 = fields[23];
		field24 = fields[24];
		field25 = fields[25];
		field26 = fields[26];
		field27 = fields[27];
		field28 = fields[28];

		if daysInMonth > 28 then field29 = fields[29]; end if;
		if daysInMonth > 29 then field30 = fields[30]; end if;
		if daysInMonth > 30 then field31 = fields[31]; end if;
		
		total_days = days_cnt || ' (' || days_cnt - full_days_cnt || ')';
		total_hours = hour_cnt || ' (' || hour_cnt - full_hour_cnt || ')'; 

		
		RETURN NEXT;
		
	END LOOP;

  END LOOP;
	

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION report_timesheet(int, int, int, int)
  OWNER TO postgres;



