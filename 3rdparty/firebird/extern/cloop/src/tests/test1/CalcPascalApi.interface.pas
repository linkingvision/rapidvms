CalcException = class(Exception)
public
	constructor create(code: Integer);

	function getCode: Integer;

	class procedure checkException(status: Status);
	class procedure catchException(status: Status; e: Exception);

private
	code: Integer;
end;

