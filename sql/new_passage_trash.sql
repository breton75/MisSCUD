DROP FUNCTION new_passage_trash(int, int, int, timestamp without time zone, int);
 
CREATE OR REPLACE FUNCTION new_passage_trash(
    IN _key_value int,
    IN _sensor_number int,
    IN _ip int,
    IN _date_time timestamp without time zone,
    IN _sensor_command int)
  RETURNS void AS

$BODY$
BEGIN

  EXECUTE 'insert into scud_history_trash (key_value, sensor_number, ip, date_time, employee_id, device_id, sensor_command) values(' ||
				_key_value     || ', '   || 
				_sensor_number || ', '   ||
				_ip            || ', ''' ||
				_date_time     || ''', ' ||
				'(select employee_id from scuds where key_value = ' || _key_value || ' limit 1), ' ||
				'(select id from devices where ip = ''' || _ip || ''' limit 1), ' ||
				_sensor_command ||
		        ')';

END;
$BODY$
  
LANGUAGE plpgsql VOLATILE
COST 100;
ALTER FUNCTION new_passage_trash(int, int, int, timestamp without time zone, int)
  OWNER TO postgres;