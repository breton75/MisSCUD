DROP FUNCTION _correction_falure_14_12_2015();

CREATE OR REPLACE FUNCTION _correction_falure_14_12_2015(
	OUT cnt int)
  RETURNS int AS

$BODY$
  declare
	r  record;


begin
  cnt = 0;
  FOR r IN EXECUTE 'SELECT distinct on (key_value) ' ||
		'	key_value, ' ||
		'	ip, ' ||
		'	employee_id, ' ||
		'	device_id ' ||
		'FROM scud_history ' ||
		'WHERE date_time between ''14/12/2015 06:00:00''::timestamp and ''14/12/2015 11:00:00''::timestamp ' ||
		'and sensor_number = 1' ||
		'and key_value not in (select key_value from scud_history where date_time between ''14/12/2015 13:00:00''::timestamp and ''15/12/2015 00:00:00''::timestamp and sensor_number = 2)'
		--|| 'and key_value = 14641885' --475926'
		
  LOOP
	cnt = cnt + 1;
	
	EXECUTE 'insert into scud_history (key_value, ip, sensor_number, date_time)' ||  --employee_id, device_id, 
		'values(' || r.key_value || ', ' || 
			r.ip || ', ' || 
			/*r.employee_id || ', ' || 
			r.device_id || ', ' || */
			'2, ''' || 
			'14/12/2015'::date + '18:00:00'::time + ((random() * 10)::int || ' minute')::interval + ((random()*100)::int || ' second')::interval || ''') ';
	

  END LOOP;


end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION _correction_falure_14_12_2015()
  OWNER TO postgres;



