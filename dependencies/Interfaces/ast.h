#pragma once

enum StatementType { LetSt, ReturnSt, ExprSt, BlockS};

typedef struct opaqueLetSt LetStatement; 

typedef struct opaqueReturnSt ReturnStatement;

typedef struct opaqueExprSt ExpressionSt;

typedef struct opaqueBlockSt BlockSt;

typedef struct opaqueStatement Statement;

typedef struct opaqueExpr Expression;

typedef struct opaqueProgram Program;