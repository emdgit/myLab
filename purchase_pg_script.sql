
---------------------------------------------------------------------------------------
---------------------------------------------------SCHEMA SERVICE ->>------------------
---------------------------------------------------------------------------------------

CREATE SCHEMA service
  AUTHORIZATION postgres;

CREATE OR REPLACE FUNCTION service.get_groups_by_parent(
	IN _parent_id integer DEFAULT NULL::integer
)
RETURNS TABLE (
	id 		integer, 
	name 		text, 
	parent_id	integer
) AS
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
  LANGUAGE plpgsql
  
  
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
  LANGUAGE plpgsql
  
  
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
LANGUAGE plpgsql
  
  
  ---------------------------------------------------------------------------------------
------------------------------------------------<< SCHEMA SERVICE ---------------------
---------------------------------------------------------------------------------------
  
---------------------------------------------------------------------------------------
---------------------------------------------------SCHEMA COMMON ->>-------------------
---------------------------------------------------------------------------------------

CREATE SCHEMA common
  AUTHORIZATION postgres;
  
  ---------------------------------------------------GROUPS
  
  CREATE TABLE common.groups
(
  id serial NOT NULL,
  group_name text NOT NULL,
  group_parent_id integer,
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
  record_id integer,
  summ real,
  date date,
  CONSTRAINT purchases_primary_key PRIMARY KEY (id),
  CONSTRAINT purchases_record_id_foreign_key FOREIGN KEY (record_id)
      REFERENCES common.records (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT purchases_user_group_foreign_key FOREIGN KEY (user_group_id)
      REFERENCES common.user_groups (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.purchases
  OWNER TO postgres;
  
  ---------------------------------------------------PURCHASE_INFO
  
  CREATE TABLE common.purchase_info
(
  purchase_id integer,
  user_id integer,
  percent real NOT NULL,
  CONSTRAINT purchase_info_purchase_foreign_key FOREIGN KEY (purchase_id)
      REFERENCES common.purchases (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT purchase_info_users_foreign_key FOREIGN KEY (user_id)
      REFERENCES common.users (id) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE NO ACTION
)
WITH (
  OIDS=FALSE
);
ALTER TABLE common.purchase_info
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
	THEN RETURN -2;
	END IF;
	
	INSERT 
	INTO common.groups ( group_name, group_parent_id )
	VALUES ( $1, $2 )
	RETURNING id INTO _result;

	RETURN _result;
END;
$BODY$
  LANGUAGE plpgsql
  
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
  LANGUAGE plpgsql
  
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
  LANGUAGE plpgsql
  
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
LANGUAGE plpgsql
  
  ---------------------------------------------------FUNC_ADD_USER_COMMENT
  
  COMMENT ON FUNCTION common.add_user(text, text, text) IS '
    Creates user with given arguments of name, info and picture. 
    Picture expected to be on device.
    It also creates group for this user which contains only him and creates record
to user_members with created user id and his group id.

Return:    [-1] - If user with given name is already exists
               Otherwise - id of added user.';
  
-----------------------------------------------------------------------------------------
------------------------------------------------<< SCHEMA COMMON ------------------------
-----------------------------------------------------------------------------------------
  
  
  
  