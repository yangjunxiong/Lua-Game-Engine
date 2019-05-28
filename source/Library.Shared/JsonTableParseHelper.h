#pragma once
#include "IJsonParseHelper.h"
#include "Stack.h"
#include "HashMap.h"
#include "Datum.h"
#include <string>
#include <memory>

namespace GameEngine
{
	class Scope;

	/// <summary>
	/// Parse a json object to a Scope object
	/// </summary>
	class JsonTableParseHelper : public IJsonParseHelper
	{
		RTTI_DECLARATIONS(JsonTableParseHelper, IJsonParseHelper);

	public:
		/// <summary>
		/// Shared data for table json parser, besides the root scope object, it also maintains a stack so that all helpers can share the state from it
		/// </summary>
		class SharedData : public BaseSharedData
		{
			RTTI_DECLARATIONS(JsonTableParseHelper::SharedData, BaseSharedData);

		public:
			/// <summary>
			/// A struct in stack to keep track of the parsing state.
			/// </summary>
			struct StackFrame
			{
				/// <summary>
				/// Current working scope
				/// </summary>
				Scope* mScope = nullptr;

				/// <summary>
				/// Whether this frame represents a datum of a nested scope. if this is a scope frame, key and valueCount will be meaningless
				/// </summary>
				bool mDatumFrame = true;

				/// <summary>
				/// If this is a datum frame, the name of this datum
				/// </summary>
				std::string mKey;

				/// <summary>
				/// If this is a datum frame, how many elements are left to parse in datum's "value" array.
				/// This is used to decide when to finish current datum and pop this frame out of stack.
				/// </summary>
				size_t mValueCount = 0;

				/// <summary>
				/// Constructor
				/// </summary>
				/// <param name="scope">Current working scope</param>
				/// <param name="datumFrame">Whether this is a datum frame</param>
				/// <param name="key">Name of the datum, if this is a datum frame</param>
				/// <param name="valueCount">Remaing number of elements to be parsed if this is a datum frame</param>
				StackFrame(Scope& scope, bool datumFrame, const std::string& key = std::string(), size_t valueCount = 0);
			};

			/// <summary>
			/// The stack to keep track of the parsing state.
			/// New frames will be pushed when the flow enters a datum object or a nested scope object, and will be poped when the flow exits corresponding object
			/// At the beginning and the end of entier parsing, the stack will contain one and only one frame, the root scope
			/// </summary>
			Stack<StackFrame> mStack;

			/// <summary>
			/// Json key to indicate this is a "type" pair
			/// </summary>
			static inline const std::string JSON_TYPE_KEY = "type";

			/// <summary>
			/// Json key to indicate this is a "value" pair
			/// </summary>
			static inline const std::string JSON_VALUE_KEY = "value";

			/// <summary>
			/// Json key to indicate this is a "class" pair
			/// </summary>
			static inline const std::string JSON_CLASS_KEY = "class";

			/// <summary>
			/// Json key to indicate this is a "file" field to replace the "value" field for a table
			/// </summary>
			static inline const std::string JSON_FILE_KEY = "file";

			/// <summary>
			/// Constructor
			/// </summary>
			/// <param name="scope">A shared pointer to a scope</param>
			explicit SharedData(const std::shared_ptr<Scope>& scope);

			/// <summary>
			/// Reset the stack to correct state before parsing or after parsing
			/// </summary>
			virtual void Initialize() override;

			/// <summary>
			/// Create a clone of this shared data, the scope will also be copied
			/// </summary>
			/// <returns>A clone of this shared data that stores a different scope object</returns>
			virtual gsl::owner<BaseSharedData*> Create() const override;

			/// <summary>
			/// Get the root scope
			/// </summary>
			/// <returns>Root scope</returns>
			Scope& GetTable();

			/// <summary>
			/// Get the root scope
			/// </summary>
			/// <returns>Root scope</returns>
			const Scope& GetTable() const;

		private:
			/// <summary>
			/// Shared pointer to root scope. We don't need to worry about the ownership problem for the table
			/// </summary>
			const std::shared_ptr<Scope> mTable;
		};

		/// <summary>
		/// Implements interface. Check if the helper can parse given json pair and parsing
		/// </summary>
		/// <param name="data">Shared data objectr</param>
		/// <param name="key">Key of json pair</param>
		/// <param name="value">Value of json pair</param>
		/// <param name="isArrayElement">Whether this value is in array</param>
		/// <param name="index">If this value is in array, the index of it</param>
		/// <returns>True if this helper can parse the pair, false otherwise</returns>
		virtual bool StartHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		
		/// <summary>
		/// Implements interface. Try to end the parsing
		/// </summary>
		/// <param name="data">Shared data object</param>
		/// <param name="key">Key of json pair</param>
		/// <param name="value">Value of json pair</param>
		/// <param name="isArrayElement">Whether this value is in array</param>
		/// <param name="index">If this value is in array, the index of it</param>
		/// <returns>True if this helper finished parsing the pair, false otherwise</returns>
		virtual bool EndHandler(BaseSharedData& data, const std::string& key, Json::Value& value, bool isArrayElement = false, size_t index = 0) override;
		
		/// <summary>
		/// Create another helper that has same type as this one
		/// </summary>
		/// <returns></returns>
		virtual gsl::owner<IJsonParseHelper*> Create() const override;

	private:
		/// <summary>
		/// Map from string to DatumType
		/// </summary>
		static const HashMap<std::string, Datum::DatumType> sDatumTypeMap;

		/// <summary>
		/// Sub function to handle starting parsing a datum from a scope
		/// </summary>
		/// <param name="data">Shared data object</param>
		/// <param name="key">Key of datum</param>
		/// <param name="value">Datum object</param>
		/// <returns>True if this is handled successfully</returns>
		bool StartTableHandler(SharedData& data, const std::string& key, Json::Value& value);

		/// <summary>
		/// Sub function to handle starting parsing a value object in a datum
		/// </summary>
		/// <param name="data">Shared data object</param>
		/// <param name="value">Value object in "value" pair in datum</param>
		/// <param name="index">Index of this value in array</param>
		/// <returns></returns>
		bool StartValueHandler(SharedData& data, Json::Value& value, size_t index);
	};
}