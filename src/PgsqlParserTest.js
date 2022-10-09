const { parse, deparse } = require('pgsql-parser');

const stmts = parse('CREATE FUNCTION my_schema.some_func(some_input bigint) AS $$ SELECT * FROM blah $$;');

// stmts[0].RawStmt.stmt.SelectStmt.fromClause[0].RangeVar.relname = 'another_table';

console.log(stmts);
console.log(stmts[0].RawStmt);
console.log(stmts[0].RawStmt.stmt.CreateFunctionStmt);

console.log('== Before ==');
console.log(deparse(stmts));

stmts[0].RawStmt.stmt.CreateFunctionStmt.replace = true;
// console.log(stmts[0].RawStmt.stmt.CreateFunctionStmt);

console.log('== After ==');
console.log(deparse(stmts));
