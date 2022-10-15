type unk;

/* type selectStmt = { */
/*   fromClause: unk, */
/*   targetList: unk, */
/*   whereClause: unk, */
/*   limitCount: unk, */
/*   limitOption: string, */
/*   op: string, */
/* }; */

type selectStmt = {
  distinctClause: unk,
  intoClause: unk,
  targetList: unk,
  fromClause: unk,
  whereClause: unk,
  groupClause: unk,
  havingClause: unk,
  windowClause: unk,
  valuesLists: unk,
  sortClause: unk,
  limitOffset: unk,
  limitCount: unk,
  lockingClause: unk,
  withClause: unk,
  op: unk,
  all: unk,
  larg: unk,
  rarg: unk,
};

type insertStmt = {
  relation: unk,
  cols: unk,
  selectStmt: unk,
  onConflictClause: unk,
  returningList: unk,
  withClause: unk,
  override: unk,
};

type updateStmt = {
  relation: unk,
  targetList: unk,
  whereClause: unk,
  fromClause: unk,
  returningList: unk,
  withClause: unk,
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
   function wrappedParse(toParse) {
     var rawParsed = PgsqlParser.parse(toParse);

     for (var i = 0; i < rawParsed.length; i++) {
       /* get key name: */
       var keyName = Object.keys(rawParsed[i].RawStmt.stmt)[0];

       /* adding `NAME`:  */
       rawParsed[i].RawStmt.stmt.NAME = keyName;

       /* adding `VAL`: */
       rawParsed[i].RawStmt.stmt.VAL = rawParsed[i].RawStmt.stmt[keyName];
     }

     return rawParsed
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
  | `SelectStmt(x) => Js.log(("Select Statement", x.fromClause))
  | `UpdateStmt(x) => Js.log(("Update Statement", x.targetList))
  | `InsertStmt(x) => Js.log(("Insert Statement", x.override))
  | `other(x) => Js.log(("other", x))
  }
);
