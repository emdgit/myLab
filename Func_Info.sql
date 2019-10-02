CREATE OR REPLACE FUNCTION test.testFunc(
	i_name		text,
	i_number	integer
)
RETURNS TABLE (
	o_id		bigint,
	o_name		text,
	o_number	integer
) AS
$$
BEGIN
	INSERT INTO test.test_table ( name, number )
	VALUES ( $1, $2 );

	RETURN QUERY

	SELECT id, name, number
	FROM test.test_table;
END;
$$
LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION test.testGetMore(
	i_number	integer
)
RETURNS TABLE (
	o_name		text
) AS
$$
BEGIN

	RETURN QUERY

	SELECT name
	FROM test.test_table
	WHERE number > $1;
END;
$$
LANGUAGE plpgsql;



CREATE OR REPLACE FUNCTION test.testGetString(
	i_date	timestamp without time zone DEFAULT NULL
)
RETURNS text AS
$$
BEGIN
	RETURN 'BAD ASS';
END;
$$
LANGUAGE plpgsql



select * from test.testGetInfo( 
	in_types_oid 	:=	'30 1028 16 26'::oidvector,
	all_types_oid	:=	'{30, 1028, 16, 26, 25, 25}'::oid[],
	is_record	:=	true,
	out_type	:=	2249,
	func_name	:=	'asdfasdfasdf'
)


CREATE OR REPLACE FUNCTION test.testGetInfo(
	in_types_oid	oidvector,
	all_types_oid	oid[],
	is_record	boolean,
	out_type	oid,
	func_name	character varying
	
)
RETURNS TABLE (
	in_arg		text,
	out_arg		text,
	f_name		character varying
) AS
$$
DECLARE
	_in_types	oid[] := $1::oid[];
	_in_count	integer := array_length( _in_types, 1 );
BEGIN
	IF ( $3 )
	THEN
		RETURN QUERY

		WITH 
			in_column AS	( select unnest( _in_types )::oid in_col ),
			out_column AS	( select unnest( $2 )::oid out_col OFFSET _in_count )
			
		SELECT DISTINCT
			ir.data_type::text		AS IN_ARG, 
			''::text			AS OUT_ARG,
			$5
		FROM	
			in_column 			AS ic,
			pg_catalog.pg_type		AS pt
		JOIN 	
			information_schema.routines	AS ir	
		ON 	
			ir.type_udt_name = pt.typname
		WHERE 	
			pt.oid = ic.in_col

		UNION ALL

		SELECT DISTINCT
			''::text			AS IN_ARG,
			ir.data_type::text		AS OUT_ARG,
			$5
		FROM	
			out_column			AS oc,
			pg_catalog.pg_type		AS pt
		JOIN 	
			information_schema.routines 	AS ir	
		ON 	
			ir.type_udt_name = pt.typname
		WHERE 	
			pt.oid = oc.out_col;
	ELSE
		RETURN QUERY

		WITH 
			in_column AS	( select unnest( _in_types )::oid in_col )
			
		SELECT DISTINCT
			ir.data_type::text		AS IN_ARG, 
			''::text			AS OUT_ARG,
			$5
		FROM	
			in_column 			AS ic,
			pg_catalog.pg_type		AS pt
		JOIN 	
			information_schema.routines 	AS ir	
		ON 	
			ir.type_udt_name = pt.typname
		WHERE 	
			pt.oid = ic.in_col
		
		UNION ALL
		
		SELECT DISTINCT
			''::text			AS IN_ARG,
			ir.data_type::text		AS OUT_ARG,
			$5
		FROM	
			pg_catalog.pg_type		AS pt
		JOIN 	
			information_schema.routines 	AS ir	
		ON 	
			ir.type_udt_name = pt.typname
		WHERE 	
			pt.oid = $4;
		
	END IF;
	
END;
$$
LANGUAGE plpgsql;


----------------------------------------------------------------------------------------------------
-------------------------------------IF OUT IS A RECORD---------------------------------------------
----------------------------------------------------------------------------------------------------
with in_par AS	( select '23 25 20'::oidvector as in_args ),
all_par AS 	( select '{23,25,20,23}'::oid[] as all_args ),
in_array AS 	( select i.in_args::oid[] AS in_arr FROM in_par AS i ),
in_column AS	( select unnest( in_arr )::oid in_col FROM in_array ),
out_column AS	( select unnest( ap.all_args )::oid out_col FROM all_par ap, in_array ia OFFSET 3 ),
out_type AS	( select 22::oid as out_type )
SELECT DISTINCT
	ir.data_type::text	AS IN_ARG, 
	''::text		AS OUT_ARG
FROM	
	in_par			AS ip,
	in_column 		AS ic,
	pg_catalog.pg_type	AS pt
JOIN 	
	information_schema.routines AS ir	
ON 	
	ir.type_udt_name = pt.typname
WHERE 	
	pt.oid = ic.in_col

UNION ALL

SELECT DISTINCT
	''::text		AS IN_ARG,
	ir.data_type::text	AS OUT_ARG
FROM	
	out_column		AS oc,
	pg_catalog.pg_type	AS pt
JOIN 	
	information_schema.routines AS ir	
ON 	
	ir.type_udt_name = pt.typname
WHERE 	
	pt.oid = oc.out_col


----------------------------------------------------------------------------------------------------
-------------------------------------IF OUT IS A SIMPLE TYPE----------------------------------------
----------------------------------------------------------------------------------------------------
with in_par AS	( select '23 25 20'::oidvector as in_args ),
all_par AS 	( select '{23,25,20,23}'::oid[] as all_args ),
in_array AS 	( select i.in_args::oid[] AS in_arr FROM in_par AS i ),
in_column AS	( select unnest( in_arr )::oid in_col FROM in_array ),
out_column AS	( select unnest( ap.all_args )::oid out_col FROM all_par ap, in_array ia OFFSET 3 ),
out_type AS	( select 1114::oid as out_t )
SELECT DISTINCT
	ir.data_type::text	AS IN_ARG, 
	''::text		AS OUT_ARG
FROM	
	in_par			AS ip,
	in_column 		AS ic,
	pg_catalog.pg_type	AS pt
JOIN 	
	information_schema.routines AS ir	
ON 	
	ir.type_udt_name = pt.typname
WHERE 	
	pt.oid = ic.in_col

UNION ALL

SELECT DISTINCT
	''::text		AS IN_ARG,
	ir.data_type::text	AS OUT_ARG
FROM	
	out_type		AS ot,
	pg_catalog.pg_type	AS pt
JOIN 	
	information_schema.routines AS ir	
ON 	
	ir.type_udt_name = pt.typname
WHERE 	
	pt.oid = ot.out_t



SELECT 
	ir.data_type::text	AS IN_ARG, 
	''::text		AS OUT_ARG
FROM	
	pg_catalog.pg_type	AS pt
JOIN 	
	information_schema.routines AS ir	
ON 	
	ir.type_udt_name = pt.typname
WHERE 	
	pt.oid = 16


SELECT 	data_type, type_udt_name
FROM	information_schema.routines
WHERE 	type_udt_name 	= 'int4' or
	data_type 	= 'int4'

-- 30 	oidvector
-- 1028	oid[]
-- 16	bool
-- 26	oid

SELECT 	*
FROM	pg_catalog.pg_type
WHERE 	oid = 1028 or oid = 16


WITH RECURSIVE t(n,i) AS (
	with arr as ( select '{1,2,3,4,5}'::integer[] as arr ),
	ind as ( select 1 as i )
	select 	
		case when ind.i < 2 then arr[i] + 500	
		else arr[i] + 1000 
		end, 1
	from arr, ind
	
	UNION ALL
	
	SELECT 
		case when i < 2 then arr[i+1] + 700	
		else arr[i+1] + 1000 
		end, i + 1
	FROM t, arr WHERE i < 5
)
SELECT * FROM t;

select * from test.testFunc( 'Third', 4257 )
select * from test.testGetMore( 100 )
select * from test.testGetString()
insert into test.test_table_2 values ( 4224, 2 )



with f_head AS (
	select 
		specific_schema	AS m_schema,
		specific_name	AS m_name,
		routine_name,
		data_type,
		type_udt_name
	from information_schema.routines
	where 
		specific_schema != 'pg_catalog' and
		specific_schema != 'information_schema'
)
select 
	(f_h).*,
	proc.proname,
	proc.pronargs		AS in_arg_count,
	proc.pronargdefaults	AS in_arg_def_count,
	proc.prorettype		AS out_type_oid,
	proc.proargtypes	AS in_types_oid,
	proc.proallargtypes	AS all_types_oid,
	proc.proargmodes,
	proc.proargnames	AS allArgNames,
	proc.proargdefaults
from pg_catalog.pg_proc as proc, f_head as f_h
where 
	proname = f_h.routine_name




with f_head AS (
		select 
			specific_schema	AS m_schema,
			routine_name,
			data_type
		from information_schema.routines
		where 
			specific_schema != 'pg_catalog' and
			specific_schema != 'information_schema'
	),
	f_body AS (
		select 
			(f_h).*,
			proc.pronargdefaults	AS in_arg_def_count,
			proc.prorettype		AS out_type_oid,
			proc.proargtypes	AS in_types_oid,
			proc.proallargtypes	AS all_types_oid,
			proc.proargnames	AS allArgNames
		from pg_catalog.pg_proc as proc, f_head as f_h
		where 
			proname = f_h.routine_name
	),
	f_names AS ( select routine_name from f_head ),
	select test.testGetInfo( 
		b.in_types_oid,
		b.all_types_oid,
		
	)
	from f_body b


	



with arr as ( select '{1,2,3,4,5}'::integer[] as arr ),
col as ( select unnest( arr )::integer ii from arr )
select array_agg( c.ii ) from col c

with arr as ( select '{1,2,3,4,5}'::integer[] as arr )
select unnest( arr )::integer ii from arr 

select case when true then 1 end, case when true then 2 end


with recursive rec(_name, _number, _func) AS (

	select name, number from test.test_table_1

)





