DROP FUNCTION report_main(integer, integer);

CREATE OR REPLACE FUNCTION report_main(
	IN _employee_id int,
	IN _department_id int,
	IN _begin timestamp without time zone,
	IN _end timestamp without time zone)
	
  RETURNS TABLE(employee_id int,
		date_time timestamp without time zone,
		device_name character varying,
		sensor_number int,
		work_time interval) AS

$BODY$
  declare
	e_r, s_r record;
	condition character varying;

begin

  if(_employee_id = -1) and (_department_id = -1) then

	condition = '';

  else if _department_id != -1 then

	condition = ' where employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ')';

  else

	condition = ' where employees.id = ' || _employee_id;

  end if;


  FOR e_r IN EXECUTE 'SELECT '
			'  scud_history.date_time, '
			'  scud_history.ip, '
			'  scud_history.key_value, '
			'  scud_history.key_value, '
			'  scud_history.sensor_number, '
'  scuds.id, '
'  departments.department_name, '
'  employees.employee_name, '
'  employees.employee_patronymic, '
'  employees.employee_surename, '
'  employees.employee_position, '
'  devices.device_name '
'FROM '
'  public.scud_history '
'  LEFT JOIN public.scuds ON scuds.key_value = scud_history.key_value '
'  LEFT JOIN public.devices ON devices.ip = scud_history.ip '
'  LEFT JOIN public.employees ON employees.id = scuds.employee_id '
'  LEFT JOIN public.departments ON departments.id = (select department_id from employees where employees.id = scuds.employee_id) '
'WHERE '



  'SELECT '
		'tasks.id as task_id, '
		'implementations.state as task_state, '
		'tasks.task_name as task_name, '
		|| login || ' as plan_laboriousness, '
		|| login || '_fact as fact_laboriousness, '
		'(select implement_stages.stage_begin from implement_stages '
		'	where implement_stages.implementation_id =(select implementations.id from implementations where implementations.task_id = tasks.id and implementations.employee_id = ' || _employee_id || ') '
		'		and implement_stages.stage_state = 0 limit 1) as task_added, '
		'(select implement_stages.stage_begin from implement_stages '
		'	where implement_stages.implementation_id =(select implementations.id from implementations where implementations.task_id = tasks.id and implementations.employee_id = ' || _employee_id || ') '
		'		and implement_stages.stage_state = 1 ORDER BY implement_stages.stage_begin ASC limit 1) as task_started, '
		'concat(employees.employee_surename, '' '', '
		'	substr(employees.employee_name, 1, 1), ''. '', '
		'	substr(employees.employee_patronymic, 1, 1), ''. '') as author '
	'FROM implementations '
	'	left join public.tasks on implementations.task_id = tasks.id '
	'	left join public.employees on implementations.employee_id = employees.id '
	'	left join public.general on implementations.task_id  = general.task_id '
	'where '
	'	tasks.project_id = ' || _project_id || ' and '
	'	implementations.employee_id = ' || _employee_id

  LOOP

	task_id = r.task_id;
	task_name = r.task_name;
	plan_laboriousness = r.plan_laboriousness;
	fact_laboriousness = r.fact_laboriousness;
	task_added = r.task_added;
	task_started = r.task_started;
	author = r.author;
	task_state = r.task_state;
	
	RETURN NEXT;
	
END LOOP;

end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION report_employee_by_project(integer, integer)
  OWNER TO postgres;



