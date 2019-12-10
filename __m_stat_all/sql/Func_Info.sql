with schemas as (
	select distinct specific_schema as _schema from information_schema.routines
	where 
		specific_schema != 'pg_catalog'	AND
		specific_schema != 'information_schema'
),
funcs as (
	select 
		r.specific_schema	as _schema,
		r.routine_name 		as _name
	from 
		information_schema.routines AS r,
		schemas AS s
	where 
		r.specific_schema = s._schema
)
select distinct
	pr.oid, 
	pr.proname,
	f._schema,
	pg_get_functiondef( pr.oid )			as def,		--получает определение функции
	pg_get_function_arguments( pr.oid )		as args,	--получает список аргументов из определения функции (со значениями по умолчанию)
	pg_get_function_identity_arguments( pr.oid )	as i_args,	--получает список аргументов, идентифицирующий функцию (без значений по умолчанию)
	pg_get_function_result( pr.oid )		as res		--получает предложение RETURNS для функции
from 
	pg_proc AS	pr,
	funcs AS 	f
where
	pr.proname = f._name
order by 
	pr.oid asc