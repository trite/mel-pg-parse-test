type t;

[@bs.module "pgsql-parser"]
external parse : string => t = "parse";

let test = parse("SELECT * FROM table1;");

[%bs.raw {|
  console.log(test[0].RawStmt)
|}]

/*
trite@DESKTOP-0ACBTNR:~/git/mel-pg-parse/_build/default/src$ node ExParse01.bs.js
{
  stmt: {
    SelectStmt: {
      targetList: [Array],
      fromClause: [Array],
      limitOption: 'LIMIT_OPTION_DEFAULT',
      op: 'SETOP_NONE'
    }
  },
  stmt_len: 20,
  stmt_location: 0
}
*/
