#pragma once
#include "Attributed.h"

namespace GameEngine
{
	class WorldState;

	/// <summary>
	/// This class is the payload type for the generic event that is scriptable from json config file.
	/// It contains a subtype field, which is used as a function name for the subscriber to match.
	/// It also carries a WorldState pointer for the subscriber to use
	/// </summary>
	class EventMessageAttributed final : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		EventMessageAttributed();

		/// <summary>
		/// Constructor that takes subtype and worldstate
		/// </summary>
		/// <param name="subtype">Subtype of this message</param>
		/// <param name="state">WorldState pointer</param>
		EventMessageAttributed(const std::string& subtype, WorldState& state);

		/// <summary>
		/// Get a clone of this object
		/// </summary>
		/// <returns>A clone of this object</returns>
		virtual gsl::owner<Scope*> Clone() const override;

		/// <summary>
		/// Get subtype of this object
		/// </summary>
		/// <returns>Subtype of this object</returns>
		const std::string& GetSubtype() const;

		/// <summary>
		/// Get WorldState pointer
		/// </summary>
		/// <returns>Pointer to WorldState object</returns>
		WorldState* GetWorldState() const;

		/// <summary>
		/// Set subtype of this object
		/// </summary>
		/// <param name="subtype">New subtype</param>
		void SetSubtype(const std::string& subtype);

		/// <summary>
		/// Set WorldState of this object
		/// </summary>
		/// <param name="state">Worldstate pointer</param>
		void SetWorldState(WorldState& state);

		/// <summary>
		/// Get the list signatures for this type
		/// </summary>
		/// <returns>The list signatures for this type</returns>
		static const Vector<Attributed::Signature> Signatures();

	private:
		/// <summary>
		/// Subtype of this object, serves as a function name match
		/// </summary>
		std::string mSubtype;

		/// <summary>
		/// Worldstate pointer, might be used by subscribers
		/// </summary>
		WorldState* mWorldState = nullptr;
	};
}
