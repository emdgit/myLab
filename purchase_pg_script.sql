﻿
---------------------------------------------------------------------------------------
---------------------------------------------------SCHEMA SERVICE ->>------------------
---------------------------------------------------------------------------------------

CREATE SCHEMA service
  AUTHORIZATION postgres;

CREATE OR REPLACE FUNCTION service.get_groups_by_parent(_parent_id integer DEFAULT NULL::integer)
  RETURNS TABLE(id integer, name text, parent_id integer, is_profit boolean) AS
$BODY$
BEGIN
	RETURN QUERY 
	
	SELECT * 
	FROM common.groups
	WHERE 
		CASE 
			WHEN $1 IS NULL
			THEN group_parent_id IS NULL
			ELSE group_parent_id = $1
		END;
END;
$BODY$
  LANGUAGE plpgsql;
  
  
CREATE OR REPLACE FUNCTION service.group_exists(group_id integer)
RETURNS boolean AS
$BODY$
BEGIN
	IF group_id IS NULL
	THEN
		RETURN FALSE;
	END IF;

	IF EXISTS ( 
		SELECT * 
		FROM common.groups
		WHERE id = $1
	)
	THEN
		RETURN TRUE;
	END IF;

	RETURN FALSE;
END;
$BODY$
  LANGUAGE plpgsql;
  
  CREATE OR REPLACE FUNCTION service.add_single_user_group (
	user_id		integer,
	user_name	text
)
RETURNS integer AS
$$
DECLARE
	_group_id	integer;
BEGIN
	_group_id := 0;
	
	INSERT
	INTO common.user_groups( group_name )
	VALUES ( $2 )
	RETURNING id INTO _group_id;

	INSERT
	INTO common.user_members
	VALUES ( $1, _group_id );

	RETURN _group_id;
END;
$$
LANGUAGE plpgsql;
  
  CREATE OR REPLACE FUNCTION service.user_group_exists (
	u_group_id	integer
)
RETURNS boolean AS
$$
BEGIN
	RETURN
	EXISTS (
		SELECT *
		FROM common.user_groups AS ug
		WHERE ug.id = $1
	);
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION service.record_exists (
	recors_id	integer
)
RETURNS boolean AS
$$
BEGIN
	RETURN
	EXISTS (
		SELECT *
		FROM common.records AS r
		WHERE r.id = $1
	);
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION service.user_exists(user_id integer)
  RETURNS boolean AS
$BODY$
BEGIN
	IF user_id IS NULL
	THEN
		RETURN FALSE;
	END IF;

	IF EXISTS ( 
		SELECT * 
		FROM common.users
		WHERE id = $1
	)
	THEN
		RETURN TRUE;
	END IF;

	RETURN FALSE;
END;
$BODY$
  LANGUAGE plpgsql;
  
CREATE OR REPLACE FUNCTION service.clear_purchases()
RETURNS boolean AS
$BODY$
BEGIN
	delete from common.purchases;
	delete from common.record_prices_stat;
	delete from common.record_stat;

	RETURN TRUE;
END;
$BODY$
  LANGUAGE plpgsql;
  
  CREATE OR REPLACE FUNCTION service.get_subgroups(IN group_id integer DEFAULT NULL::integer)
  RETURNS TABLE(id integer) AS
$BODY$
BEGIN
	return QUERY
	with recursive rec(id) as (
		select g.id 
		from common.groups g 
		where case 
			when $1 is null 
			then g.group_parent_id is null
			else g.group_parent_id = $1
			end

		union all

		select g.id 
		from 
			common.groups g, 
			rec r 
		where g.group_parent_id = r.id
	)
	select ($1) as id
	union 
	select rec.id from rec
	order by id;
END;
$BODY$
  LANGUAGE plpgsql;
  
  CREATE OR REPLACE FUNCTION service.is_record_profit(id integer)
  RETURNS boolean AS
$BODY$
DECLARE
	_res boolean := false;
BEGIN
	if exists (
		select	p.is_profit	
		from	common.records	as p
		where	p.id = $1
	) then
		select	p.is_profit
		into	_res
		from	common.records	as p
		where	p.id = $1;
		
		return _res;
	end if;

	RAISE EXCEPTION 'Nonexistent record ID --> %', $1;
END;
$BODY$
  LANGUAGE plpgsql;
  
  COMMENT ON FUNCTION service.is_record_profit(integer) IS 'Функция определяет, относится запись к прибыльным или нет.
true - прибыльная, false - нет';
  
 CREATE OR REPLACE FUNCTION service.is_group_profit(id integer)
  RETURNS boolean AS
$BODY$
DECLARE
	_res boolean := false;
BEGIN
	if exists (
		select	g.is_profit	
		from	common.groups	as g
		where	g.id = $1
	) then
		select	g.is_profit
		into	_res
		from	common.groups	as g
		where	g.id = $1;
		
		return _res;
	end if;

	RAISE EXCEPTION 'Nonexistent group ID --> %', $1;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION service.is_group_profit(integer) IS 'Функция определяет, относится группа к прибыльным или нет.
true - прибыльная, false - нет';

CREATE OR REPLACE FUNCTION service.update_accumulation_t(
    summ double precision,
    record_id integer)
  RETURNS boolean AS
$BODY$
DECLARE
	-- record existance is guaranteed here
	-- this function is called only by add_purchase trigger.
	_is_profit 	boolean := service.is_record_profit($2);
	_all_profit 	double precision;
	_all_spend	double precision;
	_clean_profit	double precision;
	_saved_percent	double precision;
BEGIN
	select 	all_profit, all_spend
	from 	common.accumulation
	into	_all_profit, _all_spend;

	if (_is_profit) then
		_all_profit := _all_profit + $1;
	else
		_all_spend := _all_spend + $1;
	end if;

	_clean_profit := _all_profit - _all_spend;

	if (_all_profit = 0.0) then
		_saved_percent := 0.0;
	else
		_saved_percent := _clean_profit * 100 / _all_profit;
	end if;
	

	update	common.accumulation
	set	all_profit = _all_profit,
		all_spend = _all_spend,
		clean_profit = _clean_profit,
		saved_percent = _saved_percent;

	RETURN TRUE;
END;
$BODY$
  LANGUAGE plpgsql;

CREATE OR REPLACE FUNCTION service.update_record_prices_add(
    record_id integer,
    summ real)
  RETURNS boolean AS
$BODY$
BEGIN
	-- Record existance MUST be guaranteed.
	if not exists (
		select 	*
		from 	common.record_prices_stat	AS rps
		where
			rps.record_id = $1		AND
			rps.summ = $2
	)
	then
		insert 
		into	common.record_prices_stat
		values	($1, $2, 1);
	else
		update 	common.record_prices_stat	AS rps
		set 	count = count + 1
		where	rps.record_id = $1		AND
			rps.summ = $2;
	end if;

	RETURN true;
END;
$BODY$
  LANGUAGE plpgsql;
ALTER FUNCTION service.update_record_prices_add(integer, real)
  OWNER TO postgres;
  
 CREATE OR REPLACE FUNCTION service.update_record_prices_del(
    record_id integer,
    summ real)
  RETURNS boolean AS
$BODY$
DECLARE
	_new_count	integer;
BEGIN
	-- Record existance MUST be guaranteed.
	if not exists (
		select 	*
		from 	common.record_prices_stat	AS rps
		where
			rps.record_id = $1		AND
			rps.summ = $2
	)
	then
		RETURN	false;
	end if;
	
	update 	common.record_prices_stat	AS rps
	set 	count = count - 1
	where	rps.record_id = $1		AND
		rps.summ = $2		
	returning count into _new_count;

	if (_new_count < 1)
	then
		delete
		from	common.record_prices_stat	AS rps
		where	rps.record_id = $1		AND
			rps.summ = $2;
	end if;

	RETURN true;
END;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION service.update_record_prices_del(integer, real)
  OWNER TO postgres;
  
  CREATE OR REPLACE FUNCTION service.update_records_stat_add(
    user_id integer,
    user_group_id integer,
    record_id integer)
  RETURNS boolean AS
$BODY$
BEGIN
	if not exists (
		select 	* 
		from 	common.record_stat	AS rs
		where 	rs.user_id = $1 	AND
			rs.user_group_id = $2	AND
			rs.record_id = $3
	)
	then
		insert 
		into 	common.record_stat
		values 	($3, $1, $2, 1);
	else
		update 	common.record_stat 	AS rs
		set 	count = count + 1
		where 	rs.user_id = $1 	AND
			rs.user_group_id = $2	AND
			rs.record_id = $3;
	end if;

	RETURN true;
END;
$BODY$
  LANGUAGE plpgsql;
ALTER FUNCTION service.update_records_stat_add(integer, integer, integer)
  OWNER TO postgres;
  
  CREATE OR REPLACE FUNCTION service.update_records_stat_del(
    user_id integer,
    user_group_id integer,
    record_id integer)
  RETURNS boolean AS
$BODY$
DECLARE
	_new_count	integer;
BEGIN
	-- Record existance MUST be guaranteed.
	if not exists (
		select 	* 
		from 	common.record_stat	AS rs
		where 	rs.user_id = $1 	AND
			rs.user_group_id = $2	AND
			rs.record_id = $3
	)
	then
		RETURN 	false;
	end if;

	update 	common.record_stat 	AS rs
	set 	count = count - 1
	where 	rs.user_id = $1 	AND
		rs.user_group_id = $2	AND
		rs.record_id = $3
	returning rs.count into _new_count;

	if (_new_count < 1)
	then
		delete
		from	common.record_stat	AS rs
		where	rs.user_id = $1 	AND
			rs.user_group_id = $2	AND
			rs.record_id = $3;
	end if;

	RETURN true;
END;
$BODY$
  LANGUAGE plpgsql;
ALTER FUNCTION service.update_records_stat_del(integer, integer, integer)
  OWNER TO postgres;

  
  ---------------------------------------------------------------------------------------
------------------------------------------------<< SCHEMA SERVICE ---------------------
---------------------------------------------------------------------------------------
  
---------------------------------------------------------------------------------------
---------------------------------------------------SCHEMA COMMON ->>-------------------
---------------------------------------------------------------------------------------

  ---------------------------------------------------PERIOD_INFO

CREATE TABLE common.period_info
(
  start_point date NOT NULL, -- Дата начала периода.
  user_group_id integer, -- В рамках какой пользовательской группы установлено начало периода
  CONSTRAINT period_info_guid_fk FOREIGN KEY (user_group_id)
      REFERENCES common.user_groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);

ALTER TABLE common.period_info
  OWNER TO postgres;
  
COMMENT ON TABLE common.period_info IS 'Информация о начале периода.';
COMMENT ON COLUMN common.period_info.start_point IS 'Дата начала периода.';
COMMENT ON COLUMN common.period_info.user_group_id IS 'В рамках какой пользовательской группы установлено начало периода';
  
  ---------------------------------------------------GROUPS
  
  CREATE TABLE common.groups
(
  id serial NOT NULL,
  group_name text NOT NULL,
  group_parent_id integer,
  is_profit boolean NOT NULL DEFAULT false,
  CONSTRAINT group_primary_key PRIMARY KEY (id),
  CONSTRAINT group_name_parent_unique UNIQUE (group_name, group_parent_id)
)
WITH (
  OIDS=FALSE
);

ALTER TABLE common.groups
  OWNER TO postgres;
  
  ---------------------------------------------------RECORDS
  
CREATE TABLE common.records
(
  id serial NOT NULL,
  group_id integer,
  name text NOT NULL,
  is_profit boolean,
  CONSTRAINT records_primary_key PRIMARY KEY (id),
  CONSTRAINT records_foreign_key FOREIGN KEY (group_id)
      REFERENCES common.groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT records_name_group_unique_key UNIQUE (group_id, name)
)
WITH (
  OIDS=FALSE
);

ALTER TABLE common.records
  OWNER TO postgres;
  
  ---------------------------------------------------USERS
  
  CREATE TABLE common.users
(
  id serial NOT NULL,
  user_name text NOT NULL,
  user_picture text,
  user_info text,
  CONSTRAINT users_primary_key PRIMARY KEY (id),
  CONSTRAINT users_unique_key UNIQUE (id, user_name)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.users
  OWNER TO postgres;
  
  ---------------------------------------------------USER_GROUPS
  
CREATE TABLE common.user_groups
(
  id serial NOT NULL,
  group_name text NOT NULL,
  CONSTRAINT user_group_primary_key PRIMARY KEY (id)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.user_groups
  OWNER TO postgres;
  
  ---------------------------------------------------USER_MEMBERS
  
  CREATE TABLE common.user_members
(
  user_id integer,
  group_id integer,
  CONSTRAINT member_user_groups_foreign_key FOREIGN KEY (group_id)
      REFERENCES common.user_groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT members_users_foreign_key FOREIGN KEY (user_id)
      REFERENCES common.users (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.user_members
  OWNER TO postgres;
  
  ---------------------------------------------------PURCHASES
  
CREATE TABLE common.purchases
(
  id serial NOT NULL,
  user_group_id integer,
  user_id integer,
  record_id integer,
  date date NOT NULL,
  creation_date date NOT NULL, -- Дата добавления записи о покупке
  summ double precision,
  is_profit boolean,
  CONSTRAINT "purchase_PK" PRIMARY KEY (id),
  CONSTRAINT "purchase_record_FK" FOREIGN KEY (record_id)
      REFERENCES common.records (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "purchase_user_FK" FOREIGN KEY (user_id)
      REFERENCES common.users (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "purchase_user_group_FK" FOREIGN KEY (user_group_id)
      REFERENCES common.user_groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.purchases
  OWNER TO postgres;
COMMENT ON COLUMN common.purchases.creation_date IS 'Дата добавления записи о покупке';
  
  
    ---------------------------------------------------RECORD_STAT
  
  
  CREATE TABLE common.record_stat
(
  record_id integer NOT NULL,
  user_id integer NOT NULL, -- Пользователь, добавляющий эапись
  user_group_id integer NOT NULL, -- Группа пользователя, находясь в которой была добавлена запись
  count integer NOT NULL, -- Сколько раз была сделана запись (по факту - количество покупок/получения дохода)
  CONSTRAINT record_stat_guid_fk FOREIGN KEY (user_group_id)
      REFERENCES common.user_groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT record_stat_record_fk FOREIGN KEY (record_id)
      REFERENCES common.records (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT record_stat_uid_fk FOREIGN KEY (user_id)
      REFERENCES common.users (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.record_stat
  OWNER TO postgres;
  
COMMENT ON TABLE common.record_stat	IS 'Статистика по добавлению записей. Частота использования записей.';
COMMENT ON COLUMN common.record_stat.user_id IS 'Пользователь, добавляющий эапись';
COMMENT ON COLUMN common.record_stat.user_group_id IS 'Группа пользователя, находясь в которой была добавлена запись';
COMMENT ON COLUMN common.record_stat.count IS 'Сколько раз была сделана запись (по факту - количество покупок/получения дохода)';


---------------------------------------------------RECORD_PRICES_STAT


CREATE TABLE common.record_prices_stat
(
  record_id integer NOT NULL, -- По какой записи производилась транзакция
  summ real NOT NULL, -- Сумма, которая имела место быть в рамках данной записи
  count integer NOT NULL DEFAULT 1, -- Сколько раз встречалась затраченная сумма в рамках этой группы
  CONSTRAINT record_price_record_fk FOREIGN KEY (record_id)
      REFERENCES common.records (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT "record_price_record-summ-unique" UNIQUE (record_id, summ)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.record_prices_stat
  OWNER TO postgres;
  
COMMENT ON TABLE common.record_prices_stat
  IS 'Таблица для ведения статистики по суммам, затраченным на каждую отдельную покупку в рамках группы. Иначе, какие стоимости присущи каждой конкретной группе и сколько раз каждая была использована. Позволит отследить средний чек по каждой конкретной записи и реализовать механизм релевантных подсказок.';
COMMENT ON COLUMN common.record_prices_stat.record_id IS 'По какой записи производилась транзакция';
COMMENT ON COLUMN common.record_prices_stat.summ IS 'Сумма, которая имела место быть в рамках данной записи';
COMMENT ON COLUMN common.record_prices_stat.count IS 'Сколько раз встречалась затраченная сумма в рамках этой группы';


---------------------------------------------------ACCUMULATION


CREATE TABLE common.accumulation
(
  all_profit double precision DEFAULT 0.0, -- Всего получено
  all_spend double precision DEFAULT 0.0, -- Всего потрачено
  clean_profit double precision DEFAULT 0.0, -- Чистая прибыль
  saved_percent double precision DEFAULT 0.0 -- Процентов сохранено
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.accumulation
  OWNER TO postgres;
COMMENT ON TABLE common.accumulation
  IS 'Накопления и характеристики за всё время';
COMMENT ON COLUMN common.accumulation.all_profit IS 'Всего получено';
COMMENT ON COLUMN common.accumulation.all_spend IS 'Всего потрачено';
COMMENT ON COLUMN common.accumulation.clean_profit IS 'Чистая прибыль';
COMMENT ON COLUMN common.accumulation.saved_percent IS 'Процентов сохранено';

insert into common.accumulation	values (0,0,0,0);



  ---------------------------------------------------PURCHASE_VIEW
  
  
  CREATE MATERIALIZED VIEW common.purchase_view AS 
 SELECT ug.group_name,
    u.user_name,
    r.name,
    p.summ,
    p.date
   FROM common.purchases p
     JOIN common.user_groups ug ON p.user_group_id = ug.id
     JOIN common.users u ON p.user_id = u.id
     JOIN common.records r ON p.record_id = r.id
WITH DATA;

ALTER TABLE common.purchase_view
  OWNER TO postgres;
  
  ---------------------------------------------------RECORD_GROUP_VIEW
  
  CREATE OR REPLACE VIEW common.record_group_view AS 
 WITH id_q AS (
         SELECT r_1.id
           FROM common.records r_1
        ), ids AS (
         SELECT q.id,
            group_id.group_id
           FROM id_q q,
            LATERAL common.get_root_group_by_record_id(q.id) group_id(group_id)
        )
 SELECT ids.id,
    ids.group_id,
    r.name AS record_name,
    g.group_name,
    g.is_profit
   FROM ids
     JOIN common.records r ON r.id = ids.id
     JOIN common.groups g ON g.id = ids.group_id;

ALTER TABLE common.record_group_view
  OWNER TO postgres;
COMMENT ON VIEW common.record_group_view
  IS 'Для просмотра соответствия записей и их корневых групп';
  
  ---------------------------------------------------FUNC_ADD_GROUP
  
CREATE OR REPLACE FUNCTION common.add_group(
    name text,
    parent_id integer DEFAULT NULL::integer,
    profit boolean DEFAULT false)
  RETURNS integer AS
$BODY$
DECLARE
	_result integer;
BEGIN
	_result := 0;

	-- Проверка наличия родительской группы
	IF $2 IS NOT NULL AND NOT EXISTS (
		SELECT * 
		FROM common.groups
		WHERE id = $2
	)
	THEN RETURN -1;
	END IF;
	
	IF EXISTS (
		SELECT * FROM (
			SELECT * 
			FROM service.get_groups_by_parent( $2 )
		) AS q
		WHERE q.name = $1
	)
	THEN
		SELECT	q.id 
		FROM 	(SELECT * FROM service.get_groups_by_parent( $2 )) AS q
		INTO	_result
		WHERE 	q.name = $1;

		RETURN _result;
	END IF;
	
	INSERT 
	INTO common.groups ( group_name, group_parent_id, is_profit )
	VALUES ( $1, $2, $3 )
	RETURNING id INTO _result;

	RETURN _result;
END;
$BODY$
  LANGUAGE plpgsql;
  
  COMMENT ON FUNCTION common.add_group(text, integer) IS 'Action    -    Adds good''s group with given name and set it''s parent to group with given parent_id.
Return    - [-1] If group with given parent_id doesn''t exists';
  
  ---------------------------------------------------FUNC_ADD_RECORD
  
CREATE OR REPLACE FUNCTION common.add_record(
    group_id integer,
    name text)
  RETURNS integer AS
$BODY$
DECLARE
	_resultId	integer;
BEGIN
	_resultId := 0;

	-- Проверка родительской группы
	IF NOT ( service.group_exists( $1 ) )
	THEN
		RETURN -1;
	END IF;

	-- Проверка существования группы. Игнорируя регистр.
	IF EXISTS ( 
		SELECT * FROM 
		common.get_records( $1 ) AS r
		WHERE lower(r.name) = lower($2)
	)
	THEN
		SELECT	id
		FROM 	common.get_records( $1 ) AS r
		INTO	_resultId
		WHERE 	lower(r.name) = lower($2);

		RETURN _resultId;
	END IF;

	INSERT 
	INTO common.records ( group_id, name, is_profit )
	VALUES ( $1, $2, service.is_group_profit($1) )
	RETURNING id INTO _resultId;

	RETURN _resultId;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_RECORDS
  
  CREATE OR REPLACE FUNCTION common.get_records(IN group_id integer)
  RETURNS TABLE(id integer, name text) AS
$BODY$
BEGIN
	RETURN QUERY

	SELECT r.id, r.name
	FROM common.records AS r
	WHERE r.group_id = $1;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_ALL_RECORDS_PROFIT
  
  CREATE OR REPLACE FUNCTION common.get_all_records_profit()
  RETURNS TABLE(id integer, group_id integer, name text) AS
$BODY$
BEGIN
	RETURN QUERY

	with group_ids AS (
		select cg.id from common.groups AS cg
		where is_profit = true
	)
	select (r).*
	from 
		common.records 	AS r,
		group_ids	AS ids
	where
		r.group_id = ids.id;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_ALL_RECORDS_SPEND
  
  CREATE OR REPLACE FUNCTION common.get_all_records_spend()
  RETURNS TABLE(id integer, group_id integer, name text) AS
$BODY$
BEGIN
	RETURN QUERY

	with group_ids AS (
		select cg.id from common.groups AS cg
		where is_profit = false
	)
	select (r).*
	from 
		common.records 	AS r,
		group_ids	AS ids
	where
		r.group_id = ids.id;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_ADD_USER
  
  CREATE OR REPLACE FUNCTION common.add_user (
	name		text,
	info		text DEFAULT ''::text,
	pic		text DEFAULT ''::text
)
RETURNS integer AS
$$
DECLARE
	_user_id	integer;
	_group_id	integer;
BEGIN
	IF EXISTS (
		SELECT * 
		FROM common.users AS u
		WHERE u.user_name = $1
	)
	THEN
		RETURN -1;
	END IF;

	INSERT 
	INTO common.users( user_name, user_info, user_picture )
	VALUES ( $1, $2, $3 )
	RETURNING id INTO _user_id;

	_group_id = service.add_single_user_group( _user_id, $1 );

	RETURN _user_id;
END;
$$
LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_ADD_USER_GROUP
  
  CREATE OR REPLACE FUNCTION common.add_user_group(
	name	text
)
RETURNS integer AS
$BODY$
DECLARE
	_group_id	integer;
BEGIN
	IF EXISTS (
		SELECT *
		FROM common.user_groups
		WHERE group_name = $1
	)
	THEN
		RETURN -1;
	END IF;

	INSERT 
	INTO common.user_groups ( group_name )
	VALUES ( $1 )
	RETURNING id INTO _group_id;

	RETURN _group_id;
END;
$BODY$
  LANGUAGE plpgsql;
  
---------------------------------------------------FUNC_ADD_PURCHASE
  
CREATE OR REPLACE FUNCTION common.add_purchase(
    u_group_id integer,
    user_id integer,
    record_id integer,
    p_summ real,
    p_date date,
    creation_date date)
  RETURNS integer AS
$BODY$
DECLARE
	_purchase_id	integer;
BEGIN
	IF NOT service.user_group_exists( $1 )
	THEN
		RETURN -1;	-- Required user_group doesn't exists
	END IF;
	IF NOT service.user_exists( $2 )
	THEN
		RETURN -2;	-- Required user doesn't exists
	END IF;

	IF NOT service.record_exists( $3 )
	THEN
		RETURN -3;	-- Required record doesn't exists
	END IF;

	INSERT
	INTO common.purchases ( user_group_id, user_id, record_id, summ, date, creation_date, is_profit )
	VALUES ( $1, $2, $3, $4, $5, $6, service.is_record_profit($3) )
	RETURNING id INTO _purchase_id;

	RETURN _purchase_id;
END;
$BODY$
  LANGUAGE plpgsql;
  
---------------------------------------------------FUNC_GET_ROOT_GROUPS_PROFIT
  
  CREATE OR REPLACE FUNCTION common.get_root_groups_profit()
  RETURNS TABLE(id integer, name text, parent_id integer) AS
$BODY$
BEGIN
	RETURN QUERY

	SELECT g.id, g.group_name, g.group_parent_id
	FROM common.groups AS g
	WHERE g.group_parent_id IS NULL AND g.is_profit = true;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_ROOT_GROUPS_SPEND
  
  CREATE OR REPLACE FUNCTION common.get_root_groups_spend()
  RETURNS TABLE(id integer, name text, parent_id integer) AS
$BODY$
BEGIN
	RETURN QUERY

	SELECT g.id, g.group_name, g.group_parent_id
	FROM common.groups AS g
	WHERE g.group_parent_id IS NULL AND g.is_profit = false;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_HAS_START_POINT
  
  CREATE OR REPLACE FUNCTION common.has_start_point(group_id integer)
RETURNS boolean AS
$BODY$
BEGIN
	if exists (
		select * from common.period_info
		where user_group_id = $1
	)
	then
		RETURN TRUE;
	else
		RETURN FALSE;
	end if;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_SET_START_POINT

CREATE OR REPLACE FUNCTION common.set_start_point(group_id integer, start_point date)
RETURNS boolean AS
$BODY$
BEGIN
	if common.has_start_point($1)
	then
		update common.period_info as pi
		set pi.start_point = $2
		where pi.user_group_id = $1;
	else
		insert into common.period_info
		values($2, $1);
	end if;

	RETURN TRUE;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_START_POINT
  
  CREATE OR REPLACE FUNCTION common.get_start_point( group_id integer )
RETURNS date AS
$BODY$
DECLARE
	_date date;
BEGIN
	if common.has_start_point($1)
	then
		select	pi.start_point
		from 	common.period_info	as pi
		into 	_date
		where 	pi.user_group_id = $1;
	else
		_date = NULL::date;
	end if;

	RETURN _date;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_GROUPS
  
  CREATE OR REPLACE FUNCTION common.get_groups( group_parent_id integer )
  RETURNS TABLE(id integer, name text, parent_id integer) AS
$BODY$
BEGIN
	RETURN QUERY

	SELECT g.id, g.group_name, g.group_parent_id
	FROM common.groups AS g
	WHERE g.group_parent_id = $1;
END;
$BODY$
  LANGUAGE plpgsql;
  
  ---------------------------------------------------FUNC_GET_ROOT_GROUPS_BY_RECORD_ID
  
  CREATE OR REPLACE FUNCTION common.get_root_group_by_record_id(record_id integer)
  RETURNS integer AS
$BODY$
DECLARE
	_group_id integer := -1;
BEGIN
	-- Проверка наличия записи
	if not exists (
		select * from common.records where id = $1
	)
	then
		return -1;
	end if;
	
	with recursive rec(id) as (
		select	r.group_id
		from   	common.records as r
		where  	r.id = $1

		union

		select 	g.group_parent_id
		from   	common.groups as g, rec
		where  	g.id = rec.id and rec.id is not null
	)
	select	* 
	from 	rec 
	into	_group_id
	where 	id is not null 
	order 	by id asc 
	limit 	1;

	return _group_id;
END;
$BODY$
  LANGUAGE plpgsql;
  
---------------------------------------------------FUNC_SUMM_PURCHASES
  
  CREATE OR REPLACE FUNCTION common.summ_purchases(
    date_from date,
    date_to date,
    group_id integer)
  RETURNS real AS
$BODY$
DECLARE 
	_parent_group 	integer;
	_result		real;
BEGIN
	if ($3 is null) 
	then	return -1.0::real;
	end if;
	
	if ($3 = 0) 
	then _parent_group := NULL::integer; 
	else _parent_group := $3;
	end if; 
	
	with recs as ( 
		select 	r.id
		from 
			service.get_subgroups(_parent_group) as sg,
			common.records as r
		where 	r.group_id = sg.id
	), purchases as (
		select	p.summ 
		from 	common.purchases as p,
			recs as r
		where 	
			p.date >= $1 		and
			p.date <= $2 		and
			p.record_id = r.id
	)
	select	sum(p.summ) 
	from 	purchases as p
	into	_result;

	return _result;
END;
$BODY$
  LANGUAGE plpgsql;
  
  COMMENT ON FUNCTION common.summ_purchases(date, date, integer) IS 'Суммирует все записи за указанный период по данной группе. 
В расчет берутся записи по всем подгруппам, включая данную.';
  
  ---------------------------------------------------FUNC_PERIOD_ROOT_GROUPS
  
  CREATE OR REPLACE FUNCTION common.period_root_groups(
    IN date_from date,
    IN date_to date,
    IN profit boolean)
  RETURNS TABLE(id integer) AS
$BODY$
BEGIN
	return 	query
	with roots as (
		select distinct	common.get_root_group_by_record_id(p.record_id) as id
		from 	common.purchases as p
		where	p.date >= $1	and
			p.date <= $2
		order by id
	), rgroups as (
		select	g.id 
		from	common.groups as g,
			roots as r
		where	g.id = r.id and g.is_profit = $3
	)
	select	*
	from	rgroups;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION common.period_root_groups(date, date, boolean) IS 'Возвращет список корневых групп для покупок, совершенных в указанный период.
Либо корневых групп прибыли, в зависимости от переданного параметра profit';
  
---------------------------------------------------FUNC_GET_PURCHASES_SUMM
  
  CREATE OR REPLACE FUNCTION common.get_purchases_summ(
    IN date_from date,
    IN date_to date,
    IN profit boolean)
  RETURNS TABLE(name text, summ double precision, last_purchase_date date) AS
$BODY$
BEGIN
	return query

	with query as (
		select	p.record_id,
			r.name, 
			sum(p.summ) as summ,
			(select unnest(array_agg(p.date)) d order by d desc limit 1) as last_date,
			r.is_profit
		from	common.purchases as p
		join 	common.records as r on r.id = p.record_id
		where	$1 <= p.date and p.date <= $2
		group by p.record_id, r.name, r.is_profit
		order by summ desc
	)
	select	q.name, q.summ, q.last_date
	from	query as q
	where	q.is_profit = $3;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION common.get_purchases_summ(date, date, boolean) IS 'Считает сумму всех записей за указанный период.';
  
---------------------------------------------------FUNC_GET_PURCHASES
  
CREATE OR REPLACE FUNCTION common.get_purchases(
    IN date_from date,
    IN date_to date,
    IN profit boolean)
  RETURNS TABLE(name text, summ double precision, last_purchase_date date) AS
$BODY$
BEGIN
	return query

	with query as (
		select	p.record_id,
			r.name, 
			p.summ,
			p.date
		from	common.purchases as p
		join 	common.records as r on r.id = p.record_id
		where	$1 <= p.date and p.date <= $2
		order by date desc
	)
	select	q.name, q.summ, q.date
	from	query as q
	where	service.is_record_profit(q.record_id) = $3;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION common.get_purchases(date, date, boolean) IS 'Отдать все транзакции за период';

---------------------------------------------------FUNC_GET_CLEAN_PROFIT

CREATE OR REPLACE FUNCTION common.get_clean_profit()
  RETURNS double precision AS
$BODY$
DECLARE
	_result double precision;
BEGIN
	select	clean_profit
	from	common.accumulation
	into	_result;

	RETURN _result;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION common.get_clean_profit() IS 'Вернуть чистую прибыль за всё время';

---------------------------------------------------FUNC_GET_SAVED_PERCENT
  
CREATE OR REPLACE FUNCTION common.get_saved_percent()
  RETURNS double precision AS
$BODY$
DECLARE
	_result double precision;
BEGIN
	select	saved_percent
	from	common.accumulation
	into	_result;

	RETURN _result;
END;
$BODY$
  LANGUAGE plpgsql;
  
COMMENT ON FUNCTION common.get_saved_percent() IS 'Вернуть процент сохраненных средств за все время';


---------------------------------------------------FUNC_GET_SUMM


CREATE OR REPLACE FUNCTION common.get_summ(
    date_from date,
    date_to date,
    profit boolean)
  RETURNS double precision AS
$BODY$
DECLARE
	_result double precision := 0.0;
BEGIN
	with summ_q as (
		select * from common.get_purchases_summ($1, $2, $3)
	)
	select 	sum(q.summ)	as spend
	into	_result
	from	summ_q		as q;

	return _result;
END;
$BODY$
  LANGUAGE plpgsql;
  
ALTER FUNCTION common.get_summ(date, date, boolean)
  OWNER TO postgres;
  
COMMENT ON FUNCTION common.get_summ(date, date, boolean) IS 'Рассчитывает сумму за данный период.
Возвращает результат одним значением.
Не берет в расчет ни группы, ни записи.';


---------------------------------------------------FUNC_REMOVE_PURCHASE_BY_ID


CREATE OR REPLACE FUNCTION common.remove_purchase_by_id(purchase_id integer)
  RETURNS boolean AS
$BODY$
DECLARE
	_is_profit	boolean;
	_summ		double precision;
	_all_profit	double precision;
	_all_spend	double precision;
	_clean		double precision;
	_saved		double precision;
BEGIN
	if exists (
		select * from common.purchases
		where id = $1
	)
	then
		select	p.is_profit, p.summ
		from 	common.purchases	as p
		into	_is_profit, _summ
		where	p.id = $1;
		
		delete from	common.purchases
		where		id = $1;

		select 	a.all_profit, a.all_spend
		from	common.accumulation	as a
		into	_all_profit, _all_spend;

		if (_is_profit)
		then
			_all_profit := _all_profit - _summ;
		else
			_all_spend := _all_spend - _summ;
		end if;

		_clean := _all_profit - _all_spend;
		_saved := _clean * 100 / _all_profit;

		update	common.accumulation
		set	all_profit = _all_profit,
			all_spend = _all_spend,
			clean_profit = _clean,
			saved_percent = _saved;
		
		RETURN TRUE;
	else
		RETURN FALSE;
	end if;
END;
$BODY$
  LANGUAGE plpgsql;
  
ALTER FUNCTION common.remove_purchase_by_id(integer) OWNER TO postgres;


  
  ---------------------------------------------------TRIGGER_FUNC_ON_PURCHASE_ADD
  
CREATE OR REPLACE FUNCTION on_purchase_add()
  RETURNS trigger AS
$BODY$
DECLARE
	_accum_result	boolean;
	_ursa		boolean;
	_urpa		boolean;
begin
	-- add record to record_stat
	_ursa := service.update_records_stat_add(NEW.user_id, 
		NEW.user_group_id, 
		NEW.record_id
	);

	-- add record to record_prices_stat
	_urpa := service.update_record_prices_add(NEW.record_id, NEW.summ::real);

	-- accumulate summ
	_accum_result := service.update_accumulation_t(NEW.summ, NEW.record_id);

	return NEW;
end;
$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION on_purchase_add()
  OWNER TO postgres;
  
  ---------------------------------------------------TRIGGER_PURCHASE_ADD
  
CREATE TRIGGER t_purchase_add
AFTER INSERT ON common.purchases FOR EACH ROW
EXECUTE PROCEDURE on_purchase_add();
  
  
  ---------------------------------------------------FUNC_ADD_USER_COMMENT
  
  COMMENT ON FUNCTION common.add_user(text, text, text) IS '
    Creates user with given arguments of name, info and picture. 
    Picture expected to be on device.
    It also creates group for this user which contains only him and creates record
to user_members with created user id and his group id.

Return:    [-1] - If user with given name is already exists
           Otherwise - id of added user.';

---------------------------------------------------FUNC_ADD_USER_GROUP_COMMENT
	       
COMMENT ON FUNCTION common.add_user_group(text) IS '
Action    - Adds empty group for users.

Return    - [-1] If group with given name is already exists.
                  - Otherwise returns added group id.';

---------------------------------------------------FUNC_ADD_PURCHASE_COMMENT

COMMENT ON FUNCTION common.add_purchase(integer, integer, integer, real, date) IS 'Action    - Adds purchase.


Return    - [-1] If user_group with given u_group_id doesn''t exists.
	  - [-2] If user with given user_id doesn''t exists.
               - [-3] If record with given record_id doesn''t exists.';

  
-----------------------------------------------------------------------------------------
------------------------------------------------<< SCHEMA COMMON ------------------------
-----------------------------------------------------------------------------------------
  
  
  
  
