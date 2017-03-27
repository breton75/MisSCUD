--DROP FUNCTION calc_employee_sum_by_day_simple(int, date);

CREATE OR REPLACE FUNCTION test_array(
	OUT f1 int,
	OUT f2 int,
	OUT f3 int)

  RETURNS record AS

$BODY$
  declare
	dd character varying array[10];
	i int;

begin



for i IN 0..10 LOOP
dd[i] = i;
end loop;

f1 = dd[0];
f2 = dd[1];
f3 = dd[10];


end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION test_array()
  OWNER TO postgres;



