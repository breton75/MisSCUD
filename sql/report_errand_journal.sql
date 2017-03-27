--DROP FUNCTION report_errand_journal(int, int, date, date);


  /* ********************************************************* */
  /* ************  ЖУРНАЛ МЕСТНЫХ КОМАНДИРОВОК  ************** */
  /* ********************************************************* */

CREATE OR REPLACE FUNCTION report_errand_journal(
	IN _employee_id int,
	IN _department_id int,
	IN _begin date,
	IN _end date)
	
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
	condition character varying;

	period_begin timestamp without time zone;
	period_final timestamp without time zone;
	day_1 date;

begin

  set datestyle to german;

  period_begin = _begin + time '00:00:00';
  period_final = _end   + time '23:59:59';

  duty = NULL; --'-<duty';
  id = NULL; --'-<id';
  lvl = NULL;
  bgnd_color = NULL;
  font_color = NULL; 
  font_bold = NULL;
  font_italic = NULL;

  field1 = '+<Сотрудник';
  field2 = '+<Должность';	
  field3 = '+|Дата командировки';
  field4 = '+|Начало и окончание';
  field5 = '+|Продолжительность';
  field6 = '+<Место командировки';

  RETURN NEXT;

  /* ШАПКА ОТЧЕТА */
  bgnd_color = x'BBFF80'::int; --BBFF80
  font_color = 0; 
  font_bold = true;
  
  field1 = 'ЖУРНАЛ МЕСТНЫХ КОМАНДИРОВОК';
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
	--field1 = 'Отдел: ' || field1;
  else
	EXECUTE 'select concat(employees.employee_surename, '' '', 
			substr(employees.employee_name, 1, 1), ''. '', 
				substr(employees.employee_patronymic, 1, 1), ''. '') as short_name from employees where id = ' || _employee_id || ' limit 1' INTO field1;
	--field1 = 'Сотрудник: ' || field1;
  end if;
  RETURN NEXT;
  
  field1 = NULL;
  RETURN NEXT;

  
  if(_employee_id = -1) and (_department_id = -1) then condition = '';
  elsif _department_id != -1 then condition = ' where departments.id = ' || _department_id;
  else condition = ' where departments.id = (select department_id from employees where employees.id = ' || _employee_id || ')';
  end if;

  FOR d_r IN EXECUTE 'SELECT id as department_id, department_name FROM departments ' || condition || ' ORDER BY department_name ASC'
  LOOP

	duty = 'department';
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
	field6 = NULL;

	RETURN NEXT;

	if(_employee_id = -1) and (_department_id = -1) then condition = ' where employees.deleted = false and employees.department_id = ' || d_r.department_id;
	elsif _department_id != -1 then condition = ' where employees.deleted = false and employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';
	else condition = ' where employees.deleted = false and employees.id = ' || _employee_id;
	end if;

	FOR p_r IN EXECUTE 'SELECT ' ||
			'	errand_journal.id as errand_journal_id, ' ||
			'	errand_journal.errand_begin, ' ||
			'	errand_journal.errand_end, ' ||
			'	errand_journal.destination, ' ||
			'	concat(employees.employee_surename, '' '', ' ||
			'		substr(employees.employee_name, 1, 1), ''. '', ' ||
			'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
			'	employees.employee_position ' ||
			'FROM errand_journal ' ||
			'LEFT JOIN employees ON employees.id = errand_journal.employee_id ' ||
			condition ||
			' and (((errand_journal.errand_begin between ''' || period_begin || ''' and ''' || period_final || ''') and (errand_journal.errand_end between ''' || period_begin || ''' and ''' || period_final || ''')) ' ||
			'	or ((''' || period_begin || ''' between errand_journal.errand_begin and errand_journal.errand_end) or (''' || period_final || ''' between errand_journal.errand_begin and errand_journal.errand_end)))' ||
			'ORDER BY errand_begin, errand_end ASC'
	LOOP

		duty = 'errand_journal';
		id = p_r.errand_journal_id;
		lvl = 1;
		bgnd_color = x'ffffff'::int; -- x'DCDCDC'::int;
		font_color = 0; 
		font_bold = false;
		font_italic = false;
		
		field1 = p_r.short_name;
		field2 = p_r.employee_position;
		field3 = p_r.errand_begin::date;
		field4 = (p_r.errand_begin::time)::char(5) || ' - ' || (p_r.errand_end::time)::char(5);
		field5 = (p_r.errand_end::time - p_r.errand_begin::time)::char(5);
		field6 = p_r.destination;
	
		RETURN NEXT;

	END LOOP;

  END LOOP;

  -- ! обязательно возвращаем стиль обратно
  set datestyle to ISO; 

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION report_errand_journal(int, int, date, date)
  OWNER TO postgres;



