#pragma once
#include <string>
#include <gsl/gsl>
#include <json/forwards.h>
#include "vector.h"

namespace GameEngine
{
	class IJsonParseHelper;
	class BaseSharedData;

	/// <summary>
	/// A coordinator to parse an input json text or file and produce outcome
	/// </summary>
	class JsonParseMaster final
	{
	public:
		/// <summary>
		/// Default constructor, create an instance with given data to fill in
		/// </summary>
		/// <param name="data">The data to fill in</param>
		explicit JsonParseMaster(BaseSharedData& data);

		/// <summary>
		/// Copy constructor is deleted, should use Clone() instead
		/// </summary>
		JsonParseMaster(const JsonParseMaster& other) = delete;

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="other">The other JsonParseMaster to move from</param>
		JsonParseMaster(JsonParseMaster&& other);

		/// <summary>
		/// Copy assignment operator is deleted, should use Clone() instead
		/// </summary>
		JsonParseMaster& operator=(const JsonParseMaster& other) = delete;

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="other">The other JsonParseMaster to move from</param>
		/// <returns>This instance after moving</returns>
		JsonParseMaster& operator=(JsonParseMaster&& other);

		/// <summary>
		/// Desctructor. If this is a cloned instance, will destruct all of its helpers and data as well
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Create a duplication of this object and return its pointer. Called is responsible to destroy it after using.
		/// The clone owns its helpers and therefore user can't modify the helper list on clone.
		/// Clone also owns its data by default, user can, however, set a new data for the clone, and the original data owned by clone will be destructed.
		/// </summary>
		/// <returns>Pointer to clone</returns>
		gsl::owner<JsonParseMaster*> Clone() const;

		/// <summary>
		/// Add a parse helper to the helper list. The helper is not added if the list contains one such helper already
		/// </summary>
		/// <param name="helper">Helper to add</param>
		/// <returns>True if the helper is added, false otherwise</returns>
		/// <exception cref="std::exception">Add helper on a cloned object</exception>
		bool AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Remove a helper from the helper list. The helper is not removed if the list doesn't contain one such helper
		/// </summary>
		/// <param name="helper">Helper to remove</param>
		/// <returns>True if helper is removed, false otherwise</returns>
		/// <exception cref="std::exception">Remove helper on a cloned object</exception>
		bool RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Try to parse the provided json string and fill in the data
		/// </summary>
		/// <param name="jsonString">The json string to parse</param>
		/// <returns>True if parsing succeeds, false otherwise</returns>
		/// <exception cref="std::exception">Json string is not valid json format</exception>
		bool Parse(const std::string& jsonString);

		/// <summary>
		/// Try to parse from input stream and fill in the data
		/// </summary>
		/// <param name="stream">The input stream to parse</param>
		/// <returns>True if parsing succeeds, false otherwise</returns>
		/// <exception cref="std::exception">stream is not valid json format</exception>
		bool Parse(std::istream& stream);

		/// <summary>
		/// Try to parse the content from a file and fill in the data
		/// </summary>
		/// <param name="path">Path to the file</param>
		/// <returns>True if parsing succeeds, false otherwise</returns>
		/// <exception cref="std::exception">File doesn't exist or doesn't contain valid json format</exception>
		bool ParseFromFile(const std::string& path);

		/// <summary>
		/// Get the name of file being parsed
		/// </summary>
		/// <returns>The name of file being parsed, by default the string is empty</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Get the shared data in this object
		/// </summary>
		/// <returns>The shared data in this object</returns>
		BaseSharedData* GetSharedData();

		/// <summary>
		/// Get shared data in this object
		/// </summary>
		/// <returns>The shared data in this object</returns>
		const BaseSharedData* GetSharedData() const;

		/// <summary>
		/// Set the shared data to a new one
		/// </summary>
		/// <param name="data">The new shared data</param>
		void SetSharedData(BaseSharedData& data);

		/// <summary>
		/// Check if this object is clone
		/// </summary>
		/// <returns>True if this object is clone, false otherwise</returns>
		bool IsClone() const;

	private:
		/// <summary>
		/// Whether this object is a clone
		/// </summary>
		bool mIsClone = false;

		/// <summary>
		/// File name being parsed
		/// </summary>
		std::string mFileName;

		/// <summary>
		/// The pointer to shared data
		/// </summary>
		BaseSharedData* mData = nullptr;

		/// <summary>
		/// List of parse helpers
		/// </summary>
		Vector<IJsonParseHelper*> mHelpers;

		/// <summary>
		/// Whether current shared data is owned by the object, it's used to handle the situation where user calls SetSharedData() on a cloned object 
		/// </summary>
		bool mDataIsClone = false;

		/// <summary>
		/// If this is a clone object, delete all objects owned by this
		/// </summary>
		void Clear();

		/// <summary>
		/// Try to parse a json key:value pair, a pair will be a concrete value, an object value or an array
		/// </summary>
		/// <param name="key">Key in the pair</param>
		/// <param name="value">Value in the pair</param>
		/// <param name="isArrayElement">Whether the value is in an array</param>
		/// <returns>True if parsed successfully, false otherwise</returns>
		bool ParsePair(const std::string& key, Json::Value& value, bool isArrayElement, size_t index);

		/// <summary>
		/// Try to parse all member in a json object
		/// </summary>
		/// <param name="value">Json object</param>
		/// <returns>True if parsed successfully, false otherwise</returns>
		bool ParseObject(Json::Value& value);
	};
}