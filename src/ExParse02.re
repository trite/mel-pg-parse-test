type t;

[@bs.module "pgsql-parser"]
external parse : string => t = "parse";

let test = parse({|
  SELECT
    a.id,
    b.id,
    a.name,
    b.name
  FROM
    table_a AS a
    JOIN table_b AS b ON b.a_id = a.id
  WHERE
    a.id = 42
  LIMIT 50
  ;
|});

[%bs.raw {|
  console.log(test[0].RawStmt)
|}];

/* [%bs.raw {| console.log(typeof(test)) |}]; */
/* [%bs.raw {| console.log(typeof(test[0])) |}]; */
/* [%bs.raw {| console.log(typeof(test[0].RawStmt)) |}]; */
/* [%bs.raw {| console.log(typeof(test)) |}] */
/* [%bs.raw {| console.log(typeof(test)) |}] */
