DROP FUNCTION new_employee(character varying,
			   character varying,
			   character varying,
			   int,
			   character varying,
			   timestamp without time zone,
			   character varying,
			   character varying,
			   character varying,
			   bytea,
			   character varying,
			   character varying);

CREATE OR REPLACE FUNCTION new_employee(
    IN _employee_name character varying,
    IN _employee_patronymic character varying,
    IN _employee_surename character varying,
    IN _department_id int,
    IN _employee_position character varying,
    IN _birth_date timestamp without time zone,
    IN _email character varying,
    IN _phone1 character varying,
    IN _phone2 character varying,
    IN _foto bytea,
    IN _description character varying,
    IN _scudkeys character varying,
    --IN _login character varying,
    OUT new_employee_id integer)
  RETURNS integer AS
$BODY$
DECLARE
	r RECORD;
begin

  if _foto is null then

	EXECUTE 'insert into employees ('		||
			'employee_name, employee_patronymic, employee_surename, department_id, employee_position, birth_date, email, phone1, phone2, description ' ||
		') values(''' 			|| 
			_employee_name 		|| ''', ''' || 
			_employee_patronymic 	|| ''', ''' || 
			_employee_surename 	|| ''', '   || 
			_department_id		|| ', '''   ||
			_employee_position 	|| ''', ''' || 
			_birth_date 		|| ''', ''' || 
			_email  		|| ''', ''' ||
			_phone1  		|| ''', ''' ||
			_phone2  		|| ''', ''' ||
			_description		|| ''')';

  else

	EXECUTE 'insert into employees ('		||
			'employee_name, employee_patronymic, employee_surename, department_id, employee_position, birth_date, email, phone1, phone2, foto, description ' ||
		') values(''' 			|| 
			_employee_name 		|| ''', ''' || 
			_employee_patronymic 	|| ''', ''' || 
			_employee_surename 	|| ''', '   || 
			_department_id		|| ', '''   ||
			_employee_position 	|| ''', ''' || 
			_birth_date 		|| ''', ''' || 
			_email  		|| ''', ''' ||
			_phone1  		|| ''', ''' ||
			_phone2  		|| ''', ''' ||
			_foto 			|| ''', ''' ||
			_description		|| ''')';

  end if;

  new_employee_id = (select max(id) from employees);

  if _scudkeys != '' then

    EXECUTE 'update scuds set employee_id = ' || new_employee_id || ' where id in (' || _scudkeys || ')';

  end if;

end;

$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION new_employee(character varying,
			    character varying,
			    character varying,
			    int,
			    character varying,
			    timestamp without time zone,
			    character varying,
			    character varying,
			    character varying,
			    bytea,
			    character varying,
			    character varying)
  OWNER TO postgres;