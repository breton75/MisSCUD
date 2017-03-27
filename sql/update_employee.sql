DROP FUNCTION update_employee(character varying,
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
				character varying,
				int);

CREATE OR REPLACE FUNCTION update_employee(
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
    IN _employee_id int)
  RETURNS void AS
  
$BODY$

declare
	cnt int = 0;
	
begin

  if _foto is null then 

	EXECUTE 'update employees set '		   ||
			'employee_name = ''' 	   || _employee_name 	   || ''', ' || 
			'employee_patronymic = ''' || _employee_patronymic || ''', ' || 
			'employee_surename = '''   || _employee_surename   || ''', ' || 
			'department_id = ' 	   || _department_id	   || ', '   ||
			'employee_position = '''   || _employee_position   || ''', ' || 
			'birth_date = ''' 	   || _birth_date 	   || ''', ' ||
			'email = ''' 	   	   || _email	 	   || ''', ' ||
			'phone1 = ''' 	   	   || _phone1	 	   || ''', ' ||
			'phone2 = ''' 	   	   || _phone2	 	   || ''', ' ||
			'description = '''	   || _description	   || ''''   ||
			'where  id = ' || _employee_id;
  else

	EXECUTE 'update employees set '		   ||
			'employee_name = ''' 	   || _employee_name 	   || ''', ' || 
			'employee_patronymic = ''' || _employee_patronymic || ''', ' || 
			'employee_surename = '''   || _employee_surename   || ''', ' || 
			'department_id = ' 	   || _department_id	   || ', '   ||
			'employee_position = '''   || _employee_position   || ''', ' || 
			'birth_date = ''' 	   || _birth_date 	   || ''', ' ||
			'email = ''' 	   	   || _email	 	   || ''', ' ||
			'phone1 = ''' 	   	   || _phone1	 	   || ''', ' ||
			'phone2 = ''' 	   	   || _phone2	 	   || ''', ' ||
			'foto = ''' 		   || _foto 		   || ''', ' ||
			'description = '''	   || _description	   || ''''   ||
			'where  id = ' || _employee_id;

  end if;


  if _scudkeys != '' then

    EXECUTE 'update scuds set employee_id = ' || _employee_id || ' where id in (' || _scudkeys || ')';
    EXECUTE 'update scuds set employee_id = -1 where employee_id = ' || _employee_id || ' and id not in (' || _scudkeys || ')';

  else

    EXECUTE 'update scuds set employee_id = -1 where employee_id = ' || _employee_id;

  end if;
			
end;

$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION update_employee(character varying,
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
				character varying,
				int)
  OWNER TO postgres;