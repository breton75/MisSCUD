DROP FUNCTION report_general(int, int, date, date, int);

CREATE OR REPLACE FUNCTION report_general(
	IN _employee_id int,
	IN _department_id int,
	IN _begin date,
	IN _end date,
	IN _report_type int)
	
  RETURNS TABLE(duty character varying,
		id int,
		--is_main_row boolean,
		lvl int,
		bgnd_color int,
		font_color int,
		font_bold boolean,
		font_italic boolean,
		
		field1 character varying,
		field2 character varying,
		field3 character varying,
		field4 character varying,
		field5 character varying,
		field6 character varying) AS

$BODY$
  declare
	d_r record;
	e_r record;
	t_r record;
	p_r record;
	day_r record;
	day_info_r record;
	condition character varying;

	period_begin timestamp without time zone;
	period_final timestamp without time zone;
	day_1 date;

	dayOfWeek int;
	holiday character varying;
	absence character varying;
	errand interval;


begin

  set datestyle to german;

  period_begin = _begin + time '00:00:00';
  period_final = _end   + time '23:59:59';

  /* ************************************* */
  /* ************  КРАТКИЙ  ************** */
  /* ************************************* */
  if _report_type = 1 then

	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+<ФИО сотрудника';
	field2 = '+<Общее время';
	field3 = '+<Должность';
	field4 = '+<';

	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 1' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;

	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;

	--font_color = x'0000FF'::int;
	if _employee_id = -1 and _department_id = -1 then field1 = 'Все сотрудники';
	elsif _employee_id = -1 then
		EXECUTE 'select department_name from departments where id = ' || _department_id || ' limit 1' INTO field1;
	else
		EXECUTE 'select concat(employees.employee_surename, '' '', 
				substr(employees.employee_name, 1, 1), ''. '', 
					substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	end if;
	RETURN NEXT;

	bgnd_color = x'FFFFFF'::int;
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */

	if(_employee_id = -1) and (_department_id = -1) then condition = '';
	elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
	else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
	end if;

	FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
	LOOP
	
		id = d_r.department_id;
		bgnd_color = x'DCDCDC'::int;
		font_color = 0; 
		font_bold = true;
		font_italic = false;

		lvl = 0;
		field1 = chr(13) || chr(10) || d_r.department_name;
		field2 = NULL;
		field3 = NULL;
		field4 = NULL;

		RETURN NEXT;

		if(_employee_id = -1) and (_department_id = -1) then condition = ' where employees.deleted = false and employees.department_id = ' || d_r.department_id;
		elsif _department_id != -1 then condition = ' where employees.deleted = false and employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
		else condition = ' where employees.deleted = false and employees.id = ' || _employee_id;
		end if;

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
			bgnd_color = x'ffffff'::int; -- x'DCDCDC'::int;
			font_color = 0; 
			font_bold = false;
			font_italic = false;
			
			field1 = e_r.short_name;
			--EXECUTE 'select total_time from calc_employee_sum_simple(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field2;
			EXECUTE 'select total_time from calc_employee_sum_first_in_last_out(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field2;
			field2 = left(field2, length(field2) - 3);
			field3 = e_r.employee_position;
			field4 = NULL; --e_r.department_name;
		
			RETURN NEXT;

		END LOOP;

	END LOOP;
	

  /* **************************************************** */
  /* ************  С ДЕТАЛИЗАЦИЕЙ ПО ДНЯМ  ************** */
  /* **************************************************** */
  elsif _report_type = 2 then

  	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+<ФИО сотрудника';
	field2 = '+<Должность';
	field3 = '+|Общее время';
	field4 = '+<Первый вход';
	field5 = '+<Последний выход';
	field6 = '+<';
	
	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 2' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;


	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;

	--font_color = x'0000FF'::int;
	if _employee_id = -1 and _department_id = -1 then field1 = 'Все сотрудники';
	elsif _employee_id = -1 then
		EXECUTE 'select department_name from departments where id = ' || _department_id || ' limit 1' INTO field1;
	else
		EXECUTE 'select concat(employees.employee_surename, '' '', 
				substr(employees.employee_name, 1, 1), ''. '', 
					substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	end if;
	RETURN NEXT;
	
	bgnd_color = x'FFFFFF'::int;
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */

	if(_employee_id = -1) and (_department_id = -1) then condition = '';
	elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
	else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
	end if;

	FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
	LOOP
	
		id = d_r.department_id;
		lvl = 0;
		bgnd_color = x'DCDCDC'::int;
		font_color = x'0000FF'::int; 
		font_bold = true;
		font_italic = false;
		
		field1 = chr(13) || chr(10) || d_r.department_name;
		field2 = NULL;
		field3 = NULL;
		field4 = NULL;
		field5 = NULL;
		field6 = NULL;

		RETURN NEXT;

		if(_employee_id = -1) and (_department_id = -1) then condition = ' where employees.deleted = false and department_id = ' || d_r.department_id;
		elsif _department_id != -1 then condition = ' where employees.deleted = false and employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
		else condition = ' where employees.deleted = false and employees.id = ' || _employee_id;
		end if;


		FOR e_r IN EXECUTE 'SELECT ' ||
				'	employees.id as employee_id, ' ||
				'	concat(employees.employee_surename, '' '', ' ||
				'		substr(employees.employee_name, 1, 1), ''. '', ' ||
				'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
				'	employees.employee_position ' ||
				--'	departments.department_name ' ||
				'FROM employees ' ||
				--'LEFT JOIN departments ON employees.department_id = departments.id ' || 
				condition ||	
				'ORDER BY short_name ASC'

		LOOP

			id = e_r.employee_id;
			lvl = 1;
			bgnd_color = x'FFFFCC'::int;
			font_color = 0; 
			font_bold = true;
			font_italic = false;
			
			field1 = e_r.short_name;
			field2 = e_r.employee_position;
			--EXECUTE 'select total_time from calc_employee_sum_simple(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field3;
			EXECUTE 'select total_time from calc_employee_sum_first_in_last_out(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field3;
			field3 = left(field3, length(field3) - 3);
			field4 = NULL;
			field5 = NULL;
			field6 = NULL;
			
			RETURN NEXT;


			day_1 = _begin;
			
			while day_1 <= _end 
			LOOP

				id = e_r.employee_id;
				lvl = 2;
				bgnd_color = x'FFFFFF'::int;
				--font_color = 0;
				
				font_bold = false;
				font_italic = false;

				/* получаем информацию по дню */
				EXECUTE 'select * from get_day_info_in_out(''' || day_1 || '''::date, ' || e_r.employee_id || ')' INTO day_info_r;

				/* если день праздничный или выходной, то выделяем красным */ 
				if day_info_r.isDayOff then font_color = x'CD4343'::int;
				else font_color = 0; end if;
				
				field1 = NULL; --day_1;
				field2 = day_1 || '    ' ||  day_info_r.dayOfWeek_designation; 

				if day_info_r.work_time is NULL and day_info_r.isDayOff then field3 = '';
				elsif day_info_r.work_time is NULL then field3 = '-';
				else field3 = (day_info_r.work_time::time)::char(5); end if;

				if day_info_r.time_in is NULL and day_info_r.isDayOff then field4 = '';
				elsif day_info_r.time_in is NULL then field4 = '-';
				else field4 = (day_info_r.time_in::time)::char(5); end if;

				if day_info_r.time_out is NULL and day_info_r.isDayOff then field5 = '';
				elsif day_info_r.time_out is NULL then field5 = '-';
				else field5 = (day_info_r.time_out::time)::char(5); end if;

				/* заполняем поле с дополнительной информацией о дне */
				field6 = NULL;

				if day_info_r.holiday is not NULL then field6 = day_info_r.holiday; end if;

				if day_info_r.absence is not NULL then
					if field6 is not NULL then field6 = field6 || ', ' || day_info_r.absence_designation; 
					else field6 = '{style: fgnd_color: 0000FF}' || day_info_r.absence_designation; end if;
				end if;

				if day_info_r.errand_time is not NULL then
					if field6 is not NULL then field6 = field6 || ', МК(' || extract(hour from day_info_r.errand_time) || ')'; 
					else field6 = '{style: fgnd_color: 0000FF}МК(' || extract(hour from day_info_r.errand_time) || ')'; end if;
				end if;
				
				
				RETURN NEXT;

				day_1 = day_1 + 1;

			END LOOP;

			id = -1;
			lvl = 2;
			bgnd_color = x'FFFFFF'::int;

			field1 = NULL;
			field2 = NULL;
			field3 = NULL;
			field4 = NULL;
			field5 = NULL;
			field6 = NULL;

			RETURN NEXT;
			
		END LOOP;
		
	END LOOP;

  /* ************************************* */
  /* **** С ДЕТАЛИЗАЦИЕЙ ПО ПРОХОДАМ  **** */
  /* ************************************* */
  elsif _report_type = 3 then

  	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+<ФИО сотрудника';
	field2 = '+<Дата/Время';
	field3 = '+<Устройство';
	field4 = '+<Вход/Выход';
	field5 = '+<Ключ      ';
	field6 = '+<';
	
	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 3' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;

	bgnd_color = x'FFFFFF'::int;
	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;

	font_color = x'0000FF'::int;
	if _employee_id = -1 and _department_id = -1 then field1 = 'Все сотрудники';
	elsif _employee_id = -1 then
		EXECUTE 'select department_name from departments where id = ' || _department_id || ' limit 1' INTO field1;
	else
		EXECUTE 'select concat(employees.employee_surename, '' '', 
				substr(employees.employee_name, 1, 1), ''. '', 
					substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	end if;
	RETURN NEXT;
  
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */

	if(_employee_id = -1) and (_department_id = -1) then
		condition = '';
	elsif _department_id != -1 then
		condition = ' where departments.id = ' || _department_id;
	else
		condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
	end if;

	FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
	LOOP
	
		id = d_r.department_id;
		lvl = 0;
		bgnd_color = x'DCDCDC'::int;
		font_color = x'0000FF'::int; 
		font_bold = true;
		font_italic = false;
		
		field1 = chr(13) || chr(10) || d_r.department_name;
		field2 = NULL;
		field3 = NULL;
		field4 = NULL;
		field5 = NULL;
		field6 = NULL;

		RETURN NEXT;

		if(_employee_id = -1) and (_department_id = -1) then
			condition = ' where employees.deleted = false and department_id = ' || d_r.department_id;
		elsif _department_id != -1 then
			condition = ' where employees.deleted = false and employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
		else
			condition = ' where employees.deleted = false and employees.id = ' || _employee_id;
		end if;


		FOR e_r IN EXECUTE 'SELECT ' ||
				'	employees.id as employee_id, ' ||
				'	concat(employees.employee_surename, '' '', ' ||
				'		substr(employees.employee_name, 1, 1), ''. '', ' ||
				'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
				'	employees.employee_position ' ||
				--'	departments.department_name ' ||
				'FROM employees ' ||
				--'LEFT JOIN departments ON employees.department_id = departments.id ' || 
				condition ||	
				'ORDER BY short_name ASC'

		LOOP

			id = e_r.employee_id;
			lvl = 1;
			bgnd_color = x'FFFFFF'::int;
			font_color = 0; 
			font_bold = true;
			font_italic = false;
			
			field1 = e_r.short_name;
			field2 = e_r.employee_position;
			--EXECUTE 'select total_time from calc_employee_sum_simple(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field3;
			EXECUTE 'select total_time from calc_employee_sum_first_in_last_out(' || e_r.employee_id || ', ''' || _begin || ''', ''' || _end || ''')' INTO field3;
			field3 = left(field3, length(field3) - 3);
			field4 = NULL;
			field5 = NULL;
			field6 = NULL;
			
			RETURN NEXT;


			FOR t_r IN EXECUTE 'SELECT ' ||
					'	scud_history.id, ' ||
					'	scud_history.sensor_number, ' ||
					'	devices.device_name, ' ||
					'	scud_history.date_time, ' ||
					'	concat(employees.employee_surename, '' '', ' ||
					'		substr(employees.employee_name, 1, 1), ''. '', ' ||
					'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
					'	employees.employee_position, ' ||
					'	scud_history.key_value ' ||
					'FROM ' ||
					'	public.scud_history ' ||
					'	LEFT JOIN public.scuds ON scuds.key_value = scud_history.key_value ' ||
					'	LEFT JOIN public.devices ON devices.ip = scud_history.ip ' ||
					'	LEFT JOIN public.employees ON employees.id = scuds.employee_id ' ||
					'WHERE ' ||
					'	scud_history.date_time >= ''' || period_begin || ''' and ' ||
					'       scud_history.date_time <= ''' || period_final || ''' and ' ||
					'       scud_history.key_value in (select scuds.key_value from scuds where scuds.employee_id = ' || e_r.employee_id || ')' ||
					'ORDER BY scud_history.date_time ASC '
			LOOP

				
				id = t_r.id;
				lvl = 2;
				bgnd_color = x'FFFFCC'::int;
				font_color = 0; 
				font_bold = false;
				font_italic = false;

				field1 = NULL;
				field2 = t_r.date_time;
				field3 = t_r.device_name;

				if t_r.sensor_number = 1 then
					field4 = '>> Вход';
					bgnd_color = x'ddffdd'::int;
				elsif t_r.sensor_number = 2 then
					field4 = '<< Выход';
					bgnd_color = x'ffdddd'::int;
				else
					field4 = '';
					bgnd_color = x'ffffff'::int;
				end if;

				field5 = t_r.key_value; --upper(to_hex(t_r.key_value))
	

				RETURN NEXT;


			END LOOP;

			id = -1;
			lvl = 2;
			bgnd_color = x'FFFFFF'::int;

			field1 = NULL;
			field2 = NULL;
			field3 = NULL;
			field4 = NULL;
			field5 = NULL;
			field6 = NULL;

			RETURN NEXT;
			
		END LOOP;
		
	END LOOP;

  /* ************************************** */
  /* ********* С ИСТОРИЯ ПРОХОДОВ ********* */
  /* ************************************** */
  elsif _report_type = 4 then

	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+|Дата/Время';
	field2 = '+<Устройство';
	field3 = '+|Вход/Выход';
	field4 = '+|Ключ';
	field5 = '+<Сотрудник';
	--field5 = '+|Должность';
	
	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 4' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;

	bgnd_color = x'FFFFFF'::int;
	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;
  
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */

	FOR t_r IN EXECUTE 'SELECT ' ||
		'	scud_history.id, ' ||
		'	scud_history.sensor_number, ' ||
		'	devices.device_name, ' ||
		'	scud_history.date_time, ' ||
		'	concat(employees.employee_surename, '' '', ' ||
		'		substr(employees.employee_name, 1, 1), ''. '', ' ||
		'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
		'	employees.employee_position, ' ||
		'	scud_history.key_value ' ||
		'FROM ' ||
		'	public.scud_history ' ||
		'	LEFT JOIN public.scuds ON scuds.key_value = scud_history.key_value ' ||
		'	LEFT JOIN public.devices ON devices.ip = scud_history.ip ' ||
		'	LEFT JOIN public.employees ON employees.id = scuds.employee_id ' ||
		'WHERE ' ||
		'	scud_history.date_time >= ''' || period_begin || ''' and scud_history.date_time <= ''' || period_final || ''' ' ||
		'ORDER BY date_time ASC '
	LOOP

		id = t_r.id;
		field1 = t_r.date_time;
		field2 = t_r.device_name;

		if t_r.sensor_number = 1 then
			field3 = '>> Вход';
			bgnd_color = x'ccffcc'::int;
		elsif t_r.sensor_number = 2 then
			field3 = '<< Выход';
			bgnd_color = x'ffcccc'::int;
		else
			field3 = '';
			bgnd_color = x'ffffff'::int;
		end if;

		field4 = t_r.key_value; --upper(to_hex(t_r.key_value));
		--field4 = lpad(field4, 6, '0');
		
		field5 = t_r.short_name || ' (' || t_r.employee_position || ')';

		RETURN NEXT;

	END LOOP;


  /* ******************************************** */
  /* *********** СПИСОК СОТРУДНИКОВ  ************ */
  /* ******************************************** */
  elsif _report_type = 5 then

	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+<Сотрудник';
	field2 = '+<Должность';
	field3 = '+<Ключ     ';
	field4 = '+<Статус   ';
	field5 = '+<';
	field6 = NULL;

	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 5' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;

	bgnd_color = x'FFFFFF'::int;
	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;

	font_color = x'0000FF'::int;
	if _employee_id = -1 and _department_id = -1 then field1 = 'Все сотрудники';
	elsif _employee_id = -1 then
		EXECUTE 'select department_name from departments where id = ' || _department_id || ' limit 1' INTO field1;
	else
		EXECUTE 'select concat(employees.employee_surename, '' '', 
				substr(employees.employee_name, 1, 1), ''. '', 
					substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	end if;
	RETURN NEXT;
  
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */

	if(_employee_id = -1) and (_department_id = -1) then condition = '';
	elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
	else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
	end if;

	FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
	LOOP
	
		id = d_r.department_id;
		bgnd_color = x'DCDCDC'::int;
		font_color = 0; 
		font_bold = true;
		font_italic = false;

		lvl = 0;
		field1 = chr(13) || chr(10) || d_r.department_name;
		field2 = NULL;
		field3 = NULL;
		field4 = NULL;
		field5 = NULL;

		RETURN NEXT;

		if (_employee_id = -1) and (_department_id = -1) then condition = ' where department_id = ' || d_r.department_id;
		elsif _department_id != -1 then condition = ' where employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
		else condition = ' where employees.id = ' || _employee_id;
		end if;


		FOR e_r IN EXECUTE 'SELECT ' ||
				'	employees.id as employee_id, ' ||
				'	concat(employees.employee_surename, '' '', ' ||
				'		substr(employees.employee_name, 1, 1), ''. '', ' ||
				'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
				'	employees.employee_position, ' ||
				'	scuds.key_value, ' ||
				'	employees.deleted ' ||
				'FROM employees ' ||
				'LEFT JOIN scuds ON scuds.employee_id = employees.id ' ||
				condition ||	
				'ORDER BY short_name ASC'

		LOOP
			id = e_r.employee_id;
			lvl = 0;
			bgnd_color = x'ffffff'::int; -- x'DCDCDC'::int;
			font_color = 0; 
			font_bold = false;
			font_italic = false;
		
			field1 = e_r.short_name;
			field2 = e_r.employee_position;
			field3 = e_r.key_value;
		
			if e_r.deleted then 
				field4 = 'Удален';
				font_color = x'ff5555'::int; 
				font_italic = true;
			else
				if field3 is NULL then 
					bgnd_color = x'ffdddd'::int; 
					field4 = 'Ключ не назначен';
				else
					field4 = '';
				end if;

				 -- bgnd_color = x'ffffcc'::int; 
				
			end if;

			field5 = NULL;
	
			RETURN NEXT;

		END LOOP;

	END LOOP;
	

  /* ************************************* */
  /* ********** СПИСОК КЛЮЧЕЙ  *********** */
  /* ************************************* */
  elsif _report_type = 6 then

	duty = NULL; --'-<duty';
	id = NULL; --'-<id';
	lvl = NULL;
	bgnd_color = NULL;
	font_color = NULL; 
	font_bold = NULL;
	font_italic = NULL;

	field1 = '+<Ключ     ';
	field2 = '+<Сотрудник';
	field3 = '+<Отдел';
	field4 = '+<Должность';
	field5 = '+<Статус';
	field6 = '+<';

	RETURN NEXT;

	/* ШАПКА ОТЧЕТА */
	bgnd_color = x'F0D080'::int;
	font_color = 0; 
	font_bold = true;
  
	EXECUTE 'select report_type_name from report_types where id = 6' INTO field1;
	field2 = NULL;
	field3 = NULL;
	field4 = NULL;
	field5 = NULL;
	field6 = NULL;
	RETURN NEXT;

	bgnd_color = x'FFFFFF'::int;
	field1 = 'Период c ' || _begin || ' по ' || _end;
	RETURN NEXT;

	font_color = x'0000FF'::int;
	if _employee_id = -1 and _department_id = -1 then field1 = 'Все сотрудники';
	elsif _employee_id = -1 then
		EXECUTE 'select department_name from departments where id = ' || _department_id || ' limit 1' INTO field1;
	else
		EXECUTE 'select concat(employees.employee_surename, '' '', 
				substr(employees.employee_name, 1, 1), ''. '', 
					substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	end if;
	RETURN NEXT;
  
	field1 = NULL;
	RETURN NEXT;
	/* КОНЕЦ ШАПКИ */
	
	if(_employee_id = -1) and (_department_id = -1) then condition = '';
	elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
	else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
	end if;

	FOR e_r IN EXECUTE 'SELECT ' ||
			'	scuds.key_value, ' ||
			'	employees.id as employee_id, ' ||
			'	concat(employees.employee_surename, '' '', ' ||
			'		substr(employees.employee_name, 1, 1), ''. '', ' ||
			'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
			'	employees.employee_position, ' ||
			'	employees.deleted, ' ||
			'	departments.department_name '
			'FROM scuds ' ||
			'LEFT JOIN employees ON scuds.employee_id = employees.id ' ||
			'LEFT JOIN departments ON employees.department_id = departments.id ' ||
			condition ||	
			'ORDER BY department_name, short_name ASC'

	LOOP
		id = e_r.employee_id;
		lvl = 0;
		bgnd_color = x'ffffff'::int; -- x'DCDCDC'::int;
		font_color = 0; 
		font_bold = false;
		font_italic = false;

		field1 = e_r.key_value;
		field2 = e_r.short_name;
		field3 = e_r.department_name;
		field4 = e_r.employee_position;
				
		if e_r.deleted then 
			field5 = 'Удален';
			font_color = x'ff5555'::int; 
			font_italic = true;
		else
			field5 = '';
		end if;

		field6 = NULL;
	
		RETURN NEXT;

	END LOOP;

  end if;
  
  -- ! обязательно возвращаем стиль обратно
  set datestyle to ISO; 

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION report_general(int, int, date, date, int)
  OWNER TO postgres;



