type node =
  | SelectStmt(outerSelectStmt)
  | FromExpr(fromExpr)
and selectStmt = {
  targetList: array(node),
  fromClause: array(node),
  whereClause: node,
  limitCount: node,
  limitOption: string,
  op: string,
}
and outerSelectStmt = {
  [@bs.as "SelectStmt"]
  selectStmt
}
and fromExpr = {
  fromList: array(node),
  quals: node,
};

type rawStmt = {
  stmt: node,
  stmt_len: int,
  stmt_location: int,
};

type outerRawStmt = {
  [@bs.as "RawStmt"]
  rawStmt: rawStmt
};

type t = array(outerRawStmt);

[@bs.module "pgsql-parser"] external parse : string => t = "parse";
[@bs.module "pgsql-parser"] external deparse : t => string = "deparse";

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

  SELECT
    *
  FROM
    blah
  ;

  UPDATE
    some_table
  SET
    some_column = "some value",
    another_column = 27
  WHERE
    id = 42
  ;

  INSERT INTO something(id, col1, col2)
  VALUES (123, "foo", "bar")
  ;
|});

Js.log(test);
Js.log(test[0]);

Js.log("=========");

/* Js.log(test[0].rawStmt); */
/* Js.log(test[0].rawStmt.stmt); */
switch(test[0].rawStmt.stmt) {
| SelectStmt(x) => Js.log(("x", x))
| FromExpr(y) => Js.log(("y", y))
}


Js.log("=========");
Js.log(test |> deparse);
