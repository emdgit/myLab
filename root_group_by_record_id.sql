select	*
from 	common.purchases as pr
where	
	pr.date >= '11-06-2020'::date and
	pr.date <= '10-07-2020'::date 

WITH RECURSIVE t(n) AS (
    VALUES (1)
  UNION ALL
    SELECT n+1 FROM t WHERE n < 100
)
SELECT sum(n) FROM t;

with recursive rec(id) as (
	select r.group_id
	from   common.records as r
	where  r.id = 11

	union

	select g.group_parent_id
	from   common.groups as g, rec
	where  g.id = rec.id and rec.id is not null
)
select * from rec where id is not null order by id asc limit 1

