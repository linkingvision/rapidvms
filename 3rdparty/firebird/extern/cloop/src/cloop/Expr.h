/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Adriano dos Santos Fernandes.
 *
 *  Copyright (c) 2014 Adriano dos Santos Fernandes <adrianosf at gmail.com>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#ifndef CLOOP_EXPR_H
#define CLOOP_EXPR_H

#include <string>


class Interface;


enum Language
{
	LANGUAGE_C,
	LANGUAGE_CPP,
	LANGUAGE_PASCAL
};


class Expr
{
public:
	virtual ~Expr()
	{
	}

public:
	virtual std::string generate(Language language, const std::string& prefix) = 0;
};


class IntLiteralExpr : public Expr
{
public:
	IntLiteralExpr(int value);

public:
	virtual std::string generate(Language language, const std::string& prefix);

private:
	int value;
};


class BooleanLiteralExpr : public Expr
{
public:
	BooleanLiteralExpr(bool value);

public:
	virtual std::string generate(Language language, const std::string& prefix);

private:
	bool value;
};


class NegateExpr : public Expr
{
public:
	NegateExpr(Expr* expr);

public:
	virtual std::string generate(Language language, const std::string& prefix);

private:
	Expr* expr;
};


class ConstantExpr : public Expr
{
public:
	ConstantExpr(Interface* interface, std::string name);

public:
	virtual std::string generate(Language language, const std::string& prefix);

private:
	Interface* interface;
	std::string name;
};


class BitwiseOrExpr : public Expr
{
public:
	BitwiseOrExpr(Expr* expr1, Expr* expr2);

public:
	virtual std::string generate(Language language, const std::string& prefix);

private:
	Expr* expr1;
	Expr* expr2;
};


#endif	// CLOOP_EXPR_H
