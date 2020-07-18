
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
  summ real,
  date date NOT NULL,
  creation_date date NOT NULL, -- Дата добавления записи о покупке
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
  
  
  
  ---------------------------------------------------FUNC_ADD_GROUP
  
CREATE OR REPLACE FUNCTION common.add_group(
    name text,
    parent_id integer DEFAULT NULL::integer)
  RETURNS integer AS
$BODY$
DECLARE
	_result integer;
BEGIN
	_result := 0;

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
	INTO common.groups ( group_name, group_parent_id )
	VALUES ( $1, $2 )
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
	
	IF NOT ( service.group_exists( $1 ) )
	THEN
		RETURN -1;
	END IF;

	IF EXISTS ( 
		SELECT * FROM 
		common.get_records( $1 ) AS r
		WHERE r.name = $2
	)
	THEN
		RETURN -2;
	END IF;

	INSERT 
	INTO common.records ( group_id, name )
	VALUES ( $1, $2 )
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
	INTO common.purchases ( user_group_id, user_id, record_id, summ, date, creation_date )
	VALUES ( $1, $2, $3, $4, $5, $6 )
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
  
  ---------------------------------------------------TRIGGER_FUNC_ON_PURCHASE_ADD
  
  CREATE OR REPLACE FUNCTION on_purchase_add()
RETURNS TRIGGER AS 
$$
BEGIN
	-- add record to record_stat
	if not exists (
		select * 
		from 
			common.record_stat	as rs
		where 
			rs.user_id = NEW.user_id AND
			rs.user_group_id = NEW.user_group_id AND
			rs.record_id = NEW.record_id
	)
	then
		insert into common.record_stat
		values (NEW.record_id, NEW.user_id, NEW.user_group_id, 1);
	else
		update common.record_stat as rs
		set count = count + 1
		where 
			rs.user_id = NEW.user_id AND
			rs.user_group_id = NEW.user_group_id AND
			rs.record_id = NEW.record_id;
	end if;

	-- add record to record_purchase_stat
	if not exists (
		select *
		from common.record_prices_stat as rps
		where
			rps.record_id = NEW.record_id AND
			rps.summ = NEW.summ
	)
	then
		insert into common.record_prices_stat
		values(NEW.record_id, NEW.summ, 1);
	else
		update common.record_prices_stat as rps
		set count = count + 1
		where
			rps.record_id = NEW.record_id AND
			rps.summ = NEW.summ;
	end if;

	RETURN NEW;
END;
$$
language plpgsql;
  
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
  
  
  
  
