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

type innerInsertStmt = unk;

type insertStmt = {
  [@bs.as "InsertStmt"]
  inner: innerInsertStmt
};

type innerUpdateStmt = unk;

type updateStmt = {
  [@bs.as "UpdateStmt"]
  inner: innerUpdateStmt
};

type stmt = [
  | `SelectStmt(selectStmt)
  | `InsertStmt(insertStmt)
  | `UpdateStmt(updateStmt)
  | `other(unk)
];

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
     var parsed = PgsqlParser.parse(blah);
     for (var i = 0; i < parsed.length; i++) {
       /* get key name: */
       var keyName = Object.keys(parsed[i].RawStmt.stmt)[0];

       /* adding `NAME`:  */
       parsed[i].RawStmt.stmt.NAME = keyName;

       /* adding `VAL`: */
       parsed[i].RawStmt.stmt.VAL = parsed[i].RawStmt.stmt[keyName];
     }
     return parsed
   }
|};
external wrappedParse : string => t = "wrappedParse";


[@bs.module "pgsql-parser"] external deparse : t => string = "deparse";
let parsed = wrappedParse({|
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

// Don't necessarily print anything here yet, but running this does 2 things:
//  1. `require("pgsql-parser")`
//      a sign I am doing something wrong probably
//  2. make sure I'm not doing something that breaks the deparse process
//      which is the point of this entire exercise
let roggle = parsed |> deparse;

parsed
-> Belt_Array.forEach(
  item =>
  switch(item.rawStmt.stmt) {
  | `SelectStmt(x) => Js.log(("Select Statement", x))
  | `UpdateStmt(x) => Js.log(("Update Statement", x))
  | `InsertStmt(x) => Js.log(("Insert Statement", x))
  | `other(x) => Js.log(("other", x))
  }
);
