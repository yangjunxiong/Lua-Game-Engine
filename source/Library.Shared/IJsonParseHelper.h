#pragma once
#include <string>
#include <gsl/gsl>
#include <json/forwards.h>
#include "RTTI.h"

namespace GameEngine
{
	class JsonParseMaster;

	/// <summary>
	/// Base abstract SharedData class that is used throughout the parsing process
	/// </summary>
	class BaseSharedData : public RTTI
	{
		RTTI_DECLARATIONS(BaseSharedData, RTTI);
		friend JsonParseMaster;

	public:
		/// <summary>
		/// Desctructor, does nothing in base
		/// </summary>
		virtual ~BaseSharedData() = default;

		/// <summary>
		/// Do some initial setup or reset the state of this data object to be ready for parsing
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// Virtual constructor that can instantiate different types of derived class
		/// </summary>
		/// <returns>A pointer to BaseSharedData object, called should delete it after using</returns>
		virtual gsl::owner<BaseSharedData*> Create() const = 0;

		/// <summary>
		/// Get the JsonParseMaster object this object belongs to
		/// </summary>
		/// <returns>The JsonParseMaster object this object belongs to</returns>
		JsonParseMaster* GetJsonParseMaster() const;

		/// <summary>
		/// Increment current nesting depth of this object
		/// </summary>
		void IncrementDepth();

		/// <summary>
		/// Decrement current nesting depth of this object
		/// </summary>
		void DecrementDepth();

		/// <summary>
		/// Get current nesting depth of this object
		/// </summary>
		/// <returns>Current nesting depth</returns>
		size_t Depth() const;

	private:
		/// <summary>
		/// Set the JsonParseMaster object this object belongs to. Meant to be called by JsonParseMater only
		/// </summary>
		/// <param name="master">The owner of this object</param>
		void SetJsonParseMaster(JsonParseMaster* master);

		/// <summary>
		/// The JsonParseMaster ths object belongs to
		/// </summary>
		JsonParseMaster* mMaster = nullptr;

		/// <summary>
		/// Current nesting depth
		/// </summary>
		size_t mDepth = 0;
	};

	/// <summary>
	/// Base abstract class for JsonParseHelper
	/// </summary>
	class IJsonParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(IJsonParseHelper, RTTI);

	public:
		/// <summary>
		/// Default constructor
		/// </summary>
		IJsonParseHelper() = default;

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other">Other helper to copy from</param>
		IJsonParseHelper(const IJsonParseHelper& other) = default;

		/// <summary>
		/// Default move constructor
		/// </summary>
		/// <param name="other">Other helper to move from</param>
		IJsonParseHelper(IJsonParseHelper&& other) = default;

		/// <summary>
		/// Default copy assignment
		/// </summary>
		/// <param name="other">Other helper to copy from</param>
		/// <returns>This object after copying</returns>
		IJsonParseHelper& operator=(const IJsonParseHelper& other) = default;

		/// <summary>
		/// Default move assignment
		/// </summary>
		/// <param name="other">Other helper to move from</param>
		/// <returns>This object after moving</returns>
		IJsonParseHelper& operator=(IJsonParseHelper&& other) = default;

		/// <summary>
		/// Default desctructor
		/// </summary>
		virtual ~IJsonParseHelper() = default;

		/// <summary>
		/// Do some initilization before parsing
		/// </summary>
		virtual void Initialize() {};

		/// <summary>
		/// Start parsing a json key:value pair and return if this helper can parse it
		/// </summary>
		/// <param name="data">The shared data to write in</param>
		/// <param name="key">Key of json pair</param>
		/// <param name="value">Value of json pair</param>
		/// <param name="isArrayElement">Whether this value is in an array</param>
		/// <returns>True if this helper can handle the pair, false otherwise</returns>
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) = 0;

		/// <summary>
		/// End parsing a json key:value pair and return if this helper successfully parsed it
		/// </summary>
		/// <param name="data">The shared data to write in</param>
		/// <param name="key">Key of json pair</param>
		/// <param name="value">Value of json pair</param>
		/// <param name="isArrayElement">Whether this value is in an array</param>
		/// <returns>True if this helper successfully handled this pair, false otherwise</returns>
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) = 0;

		/// <summary>
		/// Virtual constructor, instantiate a new instance based on derived class type
		/// </summary>
		/// <returns>The pointer to newly instantiated object, caller is responsible of deleting it after using</returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const = 0;

		/// <summary>
		/// Compare this helper with another, two are equal if they are of same type
		/// </summary>
		/// <param name="other">Other helper</param>
		/// <returns>True if two helpers are same type</returns>
		virtual bool Equals(const RTTI* other) const override;
	};
}
