{
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
}

program PascalTest;

{$ifndef FPC}
{$APPTYPE CONSOLE}
{$endif}

uses CalcPascalApi, PascalClasses,
{$ifndef FPC}
	System.SysUtils, Windows;
{$else}
	SysUtils, DynLibs;
{$endif}

type
	CreateFactoryPtr = function(): Factory; cdecl;

var
{$ifndef FPC}
	lib: THandle;
{$else}
	lib: TLibHandle;
{$endif}
	createFactory: CreateFactoryPtr;
	fact: Factory;
	stat: Status;
	calc: Calculator;
	calc2: Calculator2;
	address: Integer;
begin
{$ifndef FPC}
	lib := LoadLibrary(PWideChar(ParamStr(1)));
{$else}
	lib := LoadLibrary(ParamStr(1));
{$endif}

{$ifndef FPC}
	createFactory := GetProcAddress(lib, 'createFactory');
{$else}
	createFactory := GetProcedureAddress(lib, 'createFactory');
{$endif}

	fact := createFactory();
	stat := MyStatusImpl.create;

	calc := fact.createCalculator(stat);

	calc.sumAndStore(stat, 1, 22);
	WriteLn(calc.getMemory());	// 23

	calc.setMemory(calc.sum(stat, 2, 33));
	WriteLn(calc.getMemory());	// 35

	calc2 := fact.createCalculator2(stat);

	calc2.copyMemory(calc);
	WriteLn(calc2.getMemory());	// 35

	address := 40;
	calc2.copyMemory2(@address);
	WriteLn(calc2.getMemory());	// 35

	calc.dispose();
	calc := calc2;

	calc.sumAndStore(stat, 1, 22);
	WriteLn(calc.getMemory());	// 23

	calc.setMemory(calc.sum(stat, 2, 33));
	WriteLn(calc.getMemory());	// 35

	calc2.sumAndStore(stat, 1, 22);
	WriteLn(calc2.getMemory());	// 23

	calc2.setMemory(calc2.multiply(stat, 2, 33));
	WriteLn(calc2.getMemory());	// 66

	calc.dispose();

	calc := fact.createBrokenCalculator(stat);

	calc.sumAndStore(stat, 1, 22);
	WriteLn(calc.getMemory());	// 24

	calc.setMemory(calc.sum(stat, 2, 33));
	WriteLn(calc.getMemory());	// 36

	try
		WriteLn(calc.sum(stat, 600, 600));
	except
		on e: CalcException do
			WriteLn('exception ', e.getCode());	// exception 1
	end;

	calc.dispose();
	stat.dispose();
	fact.dispose();

	WriteLn;

{$ifndef FPC}
	FreeLibrary(lib);
{$else}
	UnloadLibrary(lib);
{$endif}
end.
