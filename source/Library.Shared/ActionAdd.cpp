#include "pch.h"
#include "ActionAdd.h"

using namespace GameEngine;
using namespace std;
RTTI_DEFINITIONS(ActionAdd);

ActionAdd::ActionAdd() :
	Action(ActionAdd::TypeIdClass())
{
	assert(mDatumPointers[OPERAND_INDEX]->first == "Operands");
}

void ActionAdd::Update(WorldState& state)
{
	Action::Update(state);

	Datum& operands = (*this)[OPERAND_INDEX];
	Datum& result = Append(RESULT_KEY).first;
	int intResult = 0;
	float floatResult = 0.f;
	string stringResult;
	Datum::DatumType type = Datum::DatumType::Unknown;

	for (uint32_t i = 0; i < operands.Size(); ++i)
	{
		Scope& scope = operands[i];

		// Differentiate value type and reference type
		assert(scope.Find("Value"));
		assert(scope.Find("Type") && scope["Type"].Type() == Datum::DatumType::Integer);
		Datum* datum;
		if (scope["Type"] == 0)  // Pure value
		{
			datum = &scope["Value"];
		}
		else                     // Reference to another datum
		{
			assert(scope["Value"].Type() == Datum::DatumType::String);
			auto [targetDatum, parentScope] = Search(scope["Value"].AsString());
			assert(targetDatum != nullptr);
			datum = targetDatum;
		}

		// Type check
		if (i == 0)
		{
			type = datum->Type();
		}
		assert(datum->Type() == type && (type == Datum::DatumType::Integer || type == Datum::DatumType::Float || type == Datum::DatumType::String));

		// Accumulate value
		if (datum->Type() == Datum::DatumType::Integer)
		{
			intResult += datum->AsInt();
		}
		else if (datum->Type() == Datum::DatumType::Float)
		{
			floatResult += datum->AsFloat();
		}
		else if (datum->Type() == Datum::DatumType::String)
		{
			stringResult += datum->AsString();
		}
	}

	if (type == Datum::DatumType::Integer)
	{
		result = intResult;
	}
	else if (type == Datum::DatumType::Float)
	{
		result = floatResult;
	}
	else
	{
		result = stringResult;
	}
}

gsl::owner<Scope*> ActionAdd::Clone() const
{
	return new ActionAdd(*this);
}

Datum& ActionAdd::Operands()
{
	return mDatumPointers[OPERAND_INDEX]->second;
}

const Datum& ActionAdd::Operands() const
{
	return mDatumPointers[OPERAND_INDEX]->second;
}

const Datum& ActionAdd::Result() const
{
	return mTable[RESULT_KEY];
}

const Vector<Attributed::Signature> ActionAdd::Signatures()
{
	return Vector<Attributed::Signature>({
		Signature("Operands", Datum::DatumType::Table, 2)
	});
}
