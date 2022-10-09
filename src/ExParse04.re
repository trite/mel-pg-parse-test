type unk;

/* type fromExpr = { */
/*   fromList: option(unk) */
/* }; */

type selectStmt = {
  fromClause: unk
};

type stmt = {
  [@bs.as "SelectStmt"]
  selectStmt: selectStmt
}

type rawStmt = {
  stmt: stmt,
  stmt_len: int,
  stmt_location: int,
};
/* type outerRawStmt = [`RawStmt(rawStmt)] */

type outerRawStmt = {
  [@bs.as "RawStmt"]
  rawStmt: rawStmt
};


/* type t = { */
/*   rawStmt: array(rawStmt) */
/* }; */

type t = array(outerRawStmt);

/* [@bs.module] external pgsqlparser : string => t = "pgsql-parser"; */
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
Js.log(test[0].rawStmt.stmt.selectStmt);
/* switch(test[0].rawStmt.stmt) { */
/* | FromExpr(y) => Js.log(("y", y)) */
/* | SelectStmt(x) => Js.log(("x", x)) */
/* } */


/* let processRawStmt = (statement: rawStmt) => */
/*   Js.log(statement); */

/* switch(test[0]) { */
/* | `RawStmt(s) => */
/*   s |> Js.log */
/* }; */




Js.log("=========");
Js.log(test |> deparse);

/* Js.log(test[0].rawStmt.stmt); */

/* Js.log(test[1].rawStmt.stmt); */
/* Js.log(test[2].rawStmt.stmt); */
/* Js.log(test[3].rawStmt.stmt); */
/* Js.log(test[0].rawStmt.stmt ); */



/* switch(test[0].rawStmt.stmt) { */
/*   | Some(SelectStmt(x)) => Js.log(("it's x", x)) */
/*   | Some(FromExpr(y)) => Js.log(("it's y", y)) */
/*   | None => Js.log("Nope") */
/* }; */



/* Js.log(test[0].rawStmt.stmt); */
/* Js.log(test[0].rawStmt.stmt); */
/* Js.log(test[0].rawStmt); */
/* [%bs.raw */
/*   {| */
/*      console.log(test[0].RawStmt) */
/*   |}] */
