DROP FUNCTION report_employees_sum(int, int, timestamp without time zone, timestamp without time zone);

CREATE OR REPLACE FUNCTION report_employees_sum(
	IN _employee_id int,
	IN _department_id int,
	IN _begin timestamp without time zone,
	IN _end timestamp without time zone)
	
  RETURNS TABLE(employee_id int,
		employee_short_name character varying,
		employee_position character varying,
		department_name character varying,
		total_work_time interval) AS

$BODY$
  declare
	r  record;
	condition character varying;

begin

  if _employee_id > -1 then 

	condition = ' where employees.id = ' || _employee_id || ' ';

  elsif _department_id > -1 then

	condition = ' where employees.id in (select employees.id from employees where employees.department_id = ' || _department_id || ') ';

  else
  
	condition = '';
	
  end if;

  FOR r IN EXECUTE 'SELECT ' ||
		'	employees.id, ' ||
		'	concat(employees.employee_surename, '' '', ' ||
		'		substr(employees.employee_name, 1, 1), ''. '', ' ||
		'		substr(employees.employee_patronymic, 1, 1), ''. '') as short_name, ' ||
		'	employees.employee_position, ' ||
		'	departments.department_name ' ||
		'FROM employees ' ||
		'LEFT JOIN departments ON employees.department_id = departments.id ' || condition ||	
		'ORDER BY short_name ASC'
  LOOP

	employee_id = r.id;
	employee_short_name = r.short_name;
	employee_position = r.employee_position;
	department_name = r.department_name;
	total_work_time = (select total_time from calc_employee_sum_precise(r.id, _begin, _end));
	RETURN NEXT;

  END LOOP;


end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION report_employees_sum(int, int, timestamp without time zone, timestamp without time zone)
  OWNER TO postgres;



