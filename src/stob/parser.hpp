#pragma once


#include <utki/Buf.hpp>
#include <papki/File.hpp>



/**
 * STOB is a very simple markup language. It is used to describe object
 * hierarchies. The only kind of objects present in STOB are Strings.
 * The name of the language comes from "STring OBjects".
 * Objects (which are strings) can have arbitrary number of child objects.
 * Example:
 * @code
 * "String object"
 * AnotherStringObject
 * "String with children"{
 *	"child 1"
 *	Child2
 *	"child three"{
 *		SubChild1
 *		"Subchild two"
 * 
 *		Property1 {value1}
 *		"Property two" {"value 2"}
 * 
 *		//comment
 * 
 *		/ * multi line
 *		   comments as in C * /
 * 
 *		"Escape sequences \" \n \r \t \\ \/"
 *	}
 * }
 * @endcode
 */
namespace stob{



/**
 * @brief Listener interface for STOB parser.
 * During the STOB document parsing the Parser notifies this listener object
 * about parsed tokens.
 */
class ParseListener{
public:
	/**
	 * @brief A string token has been parsed.
	 * This method is called by Parser when String token has been parsed.
     * @param str - parsed string.
     */
	virtual void onStringParsed(const utki::Buf<char> str = utki::Buf<char>()) = 0;
	
	/**
	 * @brief Children list parsing started.
	 * This method is called by Parser when '{' token has been parsed.
     */
	virtual void onChildrenParseStarted() = 0;
	
	/**
	 * @brief Children list parsing finished.
	 * This method is called by Parser when '}' token has been parsed.
     */
	virtual void onChildrenParseFinished() = 0;
	
	virtual ~ParseListener()noexcept{}
};



/**
 * @brief STOB Parser.
 * This is a class of STOB parser. It is used for event-based parsing of STOB
 * documents.
 */
class Parser{
	std::vector<char> stringBuf;//buffer for current string being parsed

	std::string rawStringDelimeter;//delimeter for raw string
	size_t rawStringDelimeterIndex;//index into the raw string delimeter
	
	//This variable is used for tracking current nesting level to make checks for detecting malformed STOB document
	unsigned nestingLevel;
	
	enum class State_e{
		IDLE,
		STRING_PARSED,
		QUOTED_STRING,
		ESCAPE_SEQUENCE,
		UNQUOTED_STRING,
		SINGLE_LINE_COMMENT,
		MULTILINE_COMMENT,
		RAW_STRING_OPENING_DELIMETER,
		RAW_STRING_CLOSING_DELIMETER,
		RAW_STRING
	} state;

	State_e stateAfterComment;
	
	void handleStringParsed(ParseListener& listener);
	
	void processChar(char c, ParseListener& listener);
	void processCharInIdle(char c, ParseListener& listener);
	void processCharInStringParsed(char c, ParseListener& listener);
	void processCharInUnquotedString(char c, ParseListener& listener);
	void processCharInQuotedString(char c, ParseListener& listener);
	void processCharInEscapeSequence(char c, ParseListener& listener);
	void processCharInSingleLineComment(char c, ParseListener& listener);
	void processCharInMultiLineComment(char c, ParseListener& listener);
	void processCharInRawStringOpeningDelimeter(char c, ParseListener& listener);
	void processCharInRawString(char c, ParseListener& listener);
	void processCharInRawStringClosingDelimeter(char c, ParseListener& listener);
	
public:
	/**
	 * @brief Constructor.
	 * Creates an initially reset Parser object.
     */
	Parser(){
		this->reset();
	}
	
	/**
	 * @brief Reset parser.
	 * Resets the parser to initial state, discarding all the temporary parsed data and state.
     */
	void reset();
	
	/**
	 * @brief Parse chunk of STOB data.
	 * Use this method to feed the STOB data to the parser.
     * @param chunk - data chunk to parse.
     * @param listener - listener object which will receive notifications about parsed tokens.
	 * @throw stob::Exc - in case of malformed STOB document.
     */
	void parseDataChunk(const utki::Buf<std::uint8_t> chunk, ParseListener& listener);
	
	/**
	 * @brief Finalize parsing.
	 * Call this method to finalize parsing after all the available STOB data has been fed to the parser.
	 * This will tell parser that there will be no more data and the temporary stored data should be interpreted as it is.
     * @param listener - listener object which will receive notifications about parsed tokens.
	 * @throw stob::Exc - in case of malformed STOB document.
     */
	void endOfData(ParseListener& listener);
};



/**
 * @brief Parse STOB document provided by given file interface.
 * Use this function to parse the STOB document from file.
 * @param fi - file interface to use for getting the data to parse.
 * @param listener - listener object which will receive notifications about parsed tokens.
 * @throw stob::Exc - in case of malformed STOB document.
 */
void parse(const papki::File& fi, ParseListener& listener);



}//~namespace
