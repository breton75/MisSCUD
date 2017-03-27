SELECT 
  scud_history.date_time, 
  scud_history.ip, 
  scud_history.key_value, 
  scud_history.key_value, 
  scud_history.sensor_number, 
  scuds.id,
  departments.department_name, 
  employees.employee_name, 
  employees.employee_patronymic, 
  employees.employee_surename, 
  employees.employee_position, 
  devices.device_name
FROM 
  public.scud_history
  LEFT JOIN public.scuds ON scuds.key_value = scud_history.key_value
  LEFT JOIN public.devices ON devices.ip = scud_history.ip
  LEFT JOIN public.employees ON employees.id = scuds.employee_id
  LEFT JOIN public.departments ON departments.id = (select department_id from employees where employees.id = scuds.employee_id)
WHERE 
  scud_history.ip = devices.ip AND
  scud_history.key_value = scuds.key_value AND
  employees.id = scuds.employee_id AND
  departments.id = employees.department_id;
