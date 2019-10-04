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



----------------------------------------------------------------------------------------------------
-------------------------------------FUNCTION INFO--------------------------------------------------
----------------------------------------------------------------------------------------------------
--DROP FUNCTION test.testgetinfo(oidvector, oid[], boolean, oid, character varying);
CREATE OR REPLACE FUNCTION test.testGetInfo1(
	in_types_oid	oidvector,
	all_types_oid	oid[],
	is_record	boolean,
	out_type	oid,
	func_name	character varying
	
)
RETURNS SETOF RECORD AS
$$
DECLARE
	_in_types	oid[] := $1::oid[];
	_in_count	integer := array_length( _in_types, 1 );
	_rec		record;
BEGIN
	IF ( $3 )
	THEN		-- is record

		FOR i IN 0..( _in_count - 1 ) LOOP
			WITH 
				in_column AS	( select unnest( _in_types )::oid in_col )
				
			SELECT DISTINCT
				ir.data_type::text		AS IN_ARG, 
				''::text			AS OUT_ARG,
				$5
			INTO	_rec
			FROM	
				in_column 			AS ic,
				pg_catalog.pg_type		AS pt
			JOIN 	
				information_schema.routines	AS ir	
			ON 	
				ir.type_udt_name = pt.typname
			WHERE 	
				pt.oid = ic.in_col
			OFFSET	i
			LIMIT	1;

			RETURN NEXT _rec;
		END LOOP;

		FOR i IN _in_count..NULLIF(0, array_length( $2, 1 )) LOOP
			WITH 
				all_types AS	( select unnest( $2 )::oid all_col )
				
			SELECT DISTINCT
				''::text			AS IN_ARG,
				ir.data_type::text		AS OUT_ARG, 
				$5
			INTO	_rec
			FROM	
				all_types 			AS at,
				pg_catalog.pg_type		AS pt
			JOIN 	
				information_schema.routines	AS ir	
			ON 	
				ir.type_udt_name = pt.typname
			WHERE 	
				pt.oid = at.all_col
			OFFSET	i
			LIMIT	1;

			RETURN NEXT _rec;
		END LOOP;
	ELSE
		FOR i IN 0..( _in_count - 1 ) LOOP
			WITH 
				in_column AS	( select unnest( _in_types )::oid in_col )
				
			SELECT DISTINCT
				ir.data_type::text		AS IN_ARG, 
				''::text			AS OUT_ARG,
				$5
			INTO	_rec
			FROM	
				in_column 			AS ic,
				pg_catalog.pg_type		AS pt
			JOIN 	
				information_schema.routines	AS ir	
			ON 	
				ir.type_udt_name = pt.typname
			WHERE 	
				pt.oid = ic.in_col
			OFFSET	i
			LIMIT	1;

			RETURN NEXT _rec;
		END LOOP;


		WITH 
				in_column AS	( select unnest( _in_types )::oid in_col )
				
		SELECT DISTINCT
			''::text			AS IN_ARG,
			ir.data_type::text		AS OUT_ARG, 
			$5
		INTO	_rec
		FROM	
			in_column 			AS ic,
			pg_catalog.pg_type		AS pt
		JOIN 	
			information_schema.routines	AS ir	
		ON 	
			ir.type_udt_name = pt.typname
		WHERE 	
			pt.oid = $4;

		RETURN NEXT _rec;
		
	END IF;
	
END;
$$
LANGUAGE plpgsql;






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



----------------------------------------------------------------------------------------------------
-------------------------------------FUNCTION TYPES FULL--------------------------------------------
----------------------------------------------------------------------------------------------------
with f_head 	AS (
	select 
		specific_schema	AS m_schema,
		routine_name,
		data_type,
		type_udt_name
	from information_schema.routines
	where 
		specific_schema != 'pg_catalog' and
		specific_schema != 'information_schema'
),
f_body 		AS (
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
f_names		AS ( select routine_name from f_head )
select  
		(b).*,
		(res).*
from 		
		f_body AS b, 
		test.testGetInfo( 
			b.in_types_oid,
			b.all_types_oid,
			b.type_udt_name = 'record',
			b.out_type_oid,
			b.routine_name
		) AS res( 
			_in 	text, 
			_out	text,
			_func	character varying 
		)
--




----------------------------------------------------------------------------------------------------
-------------------------------------FUNCTION TYPES SHORT-------------------------------------------
----------------------------------------------------------------------------------------------------
with f_head 	AS (
	select 
		specific_schema	AS m_schema,
		routine_name,
		data_type,
		type_udt_name
	from information_schema.routines
	where 
		specific_schema != 'pg_catalog' and
		specific_schema != 'information_schema'
),
f_body 		AS (
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
f_names		AS ( select routine_name from f_head )
select  
		b.m_schema,
		b.routine_name,
		res._in,
		res._out
from 		
		f_body AS b, 
		test.testGetInfo1( 
			b.in_types_oid,
			b.all_types_oid,
			b.type_udt_name = 'record',
			b.out_type_oid,
			b.routine_name
		) AS res( 
			_in 	text, 
			_out	text,
			_func	character varying 
		)


