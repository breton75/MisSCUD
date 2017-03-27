select 
	devices.id,
	devices.ip,
	devices.device_name
from
devices

where ip in (SELECT ip FROM devices group by ip having count(*) > 1)

