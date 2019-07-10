#include "pch.h"
#include "ContentTypeReader.h"

using namespace std;

namespace GameEngine
{
	RTTI_DEFINITIONS(AbstractContentTypeReader)

	uint64_t AbstractContentTypeReader::TargetTypeId() const
	{
		return mTargetTypeId;
	}

	AbstractContentTypeReader::AbstractContentTypeReader(Game& game, const uint64_t targetTypeId) :
		mGame(&game), mTargetTypeId(targetTypeId)
	{
	}
}