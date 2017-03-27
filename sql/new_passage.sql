DROP FUNCTION new_passage(int, int, int, timestamp without time zone);
 
CREATE OR REPLACE FUNCTION new_passage(
    IN _key_value int,
    IN _sensor_number int,
    IN _ip int,
    IN _date_time timestamp without time zone)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'insert into scud_history (key_value, sensor_number, ip, date_time, employee_id, device_id) values(' ||
				_key_value     || ', '   || 
				_sensor_number || ', '   ||
				_ip            || ', ''' ||
				_date_time     || ''', ' ||
				'(select employee_id from scuds where key_value = ' || _key_value || ' limit 1), ' ||
				'(select id from devices where ip = ''' || _ip || ''' limit 1)' ||
		        ')';

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_passage(int, int, int, timestamp without time zone)
  OWNER TO postgres;