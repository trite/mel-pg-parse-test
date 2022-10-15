type unk;

type innerSelectStmt = {
  fromClause: unk,
  targetList: unk,
  whereClause: unk,
  limitCount: unk,
  limitOption: string,
  op: string,
};

type selectStmt = {
  [@bs.as "SelectStmt"]
  inner: innerSelectStmt
};

/* type stmt = { */
/*   [@bs.as "SelectStmt"] */
/*   selectStmt: innerSelectStmt, */
/*   [@bs.as "UpdateStmt"] */
/*   updateStmt: unk */
/* } */

/* type stmt = [ */
/*   | [@bs.as "SelectStmtrrrr"] `SelectStmt(innerSelectStmt) */
/*   | `UpdateStmt(unk) */
/* ]; */
/* type stmt = */
/*   | SelectStmt(innerSelectStmt) */
/*   | UpdateStmt(unk) */
/* ; */

/* [@bs.unwrap] */

/* [@bs.deriving accessors] */
/* type stmt = */
/*   | SelectStmt(selectStmt) */
/*   | UpdateStmt(unk) */
/* ; */
type stmt = [
  | `selectStmt(innerSelectStmt)
  | `other(string)
];

/* [@bs.val] */
/* external stmtTest: */
/*   [@bs.unwrap] [ */
/*     | `selectStmt(innerSelectStmt) */
/*     | `other(string) */
/*   ] = "blah"; */

type rawStmt = {
  stmt: stmt,
  stmt_len: int,
  stmt_location: int,
};

type outerRawStmt = {
  [@bs.as "RawStmt"]
  rawStmt: rawStmt
};

type t = array(outerRawStmt);

%raw {|
   function wrappedParse(blah) {
     console.log(PgsqlParser.parse(blah));
   }
|}
external wrappedParse : string => unit = "wrappedParse";

wrappedParse("SELECT * FROM blah;");

[@bs.module "pgsql-parser"] external parse : string => t = "parse";
/* [@bs.module "pgsql-parser"] external deparse : t => string = "deparse"; */
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
/*
[
  { RawStmt: { stmt: [Object], stmt_len: 151, stmt_location: 0 } },
  { RawStmt: { stmt: [Object], stmt_len: 35, stmt_location: 152 } },
  { RawStmt: { stmt: [Object], stmt_len: 110, stmt_location: 188 } },
  { RawStmt: { stmt: [Object], stmt_len: 73, stmt_location: 299 } }
]
*/

Js.log(test[0]);
/*
{
  RawStmt: { stmt: { SelectStmt: [Object] }, stmt_len: 151, stmt_location: 0 }
}
*/

Js.log(test[0].rawStmt);
/*
{
  stmt: {
    SelectStmt: {
      targetList: [Array],
      fromClause: [Array],
      whereClause: [Object],
      limitCount: [Object],
      limitOption: 'LIMIT_OPTION_COUNT',
      op: 'SETOP_NONE'
    }
  },
  stmt_len: 151,
  stmt_location: 0
}
*/

Js.log(test[0].rawStmt.stmt);

switch(test[0].rawStmt.stmt) {
| `selectStmt(x) => Js.log(("Select Statement", x))
| `other(x) => Js.log(("other", x))
};


/* switch(test[0].rawStmt.stmt) { */
/* | UpdateStmt(y) => Js.log(("Update Statement", y)) */
/* | SelectStmt(x) => Js.log(("Select Statement", x)) */
/* }; */

/* Js.log(test[0].rawStmt.stmt) */

/*
[ 'Update Statement', undefined ]
*/
