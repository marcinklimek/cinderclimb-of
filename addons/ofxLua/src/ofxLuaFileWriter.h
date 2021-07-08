/*
 * Copyright (c) 2012 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxLua for documentation
 *
 */
#pragma once

#include "lua.hpp"

#include "ofLog.h"
#include "ofConstants.h"

/// writes variables out to a lua text file
///
/// this is a not a scripting replacement, but a quick way to store data
///
/// this class is largely derived from the Allacrost scripting system: http://allacrost.sourceforge.net
///
class ofxLuaFileWriter {

	public:

		ofxLuaFileWriter();
		~ofxLuaFileWriter();
		
		/// save the current buffer to a file,
		/// best to name it with the ".lua" ext
		bool saveToFile(const std::string& filename);
		
		/// clear the buffer
		void clear();

	/// \section Write elements
	
		/// insert a new line char
		void newLine();
	
		/// writes a single line "--" comment
		void writeComment(const std::string& comment);
		
		/// starts and stops a multi-line comment block aka
		/// --[[
		/// 	some commments, etc
		/// --]]
		///
		/// note: all data written after beginCommentBlock() will be comments
		void beginCommentBlock();
		void endCommentBlock();
		
		/// write a single line of text, not variable data
		///
		/// note: don't write close comments "--]]" when in a comment block
		void writeLine(const std::string& comment);
		
		/// create a table, subsequent data is written as variables inside
		void beginTable(const std::string& tableName);
		void beginTable(const unsigned int tableIndex); //< must beginTable first
		void endTable();

	/// \section Write variables
		
		void writeBool(const std::string& name, bool value);
		void writeBool(const unsigned int index, bool value);
		
		void writeNumber(const std::string& name, lua_Number value);
		void writeNumber(const unsigned int index, lua_Number value);
		
		void writeString(const std::string& name, std::string value);
		void writeString(const unsigned int index, std::string value);

		void writeBoolVector(const std::string& tableName, std::vector<bool>& v);
		void writeBoolVector(const unsigned int index, std::vector<bool>& v);
		
		void writeNumberVector(const std::string& tableName, std::vector<lua_Number>& v);
		void writeNumberVector(const unsigned int index, std::vector<lua_Number>& v);
		
		void writeStringVector(const std::string& tableName, std::vector<std::string>& v);
		void writeStringVector(const unsigned int index, std::vector<std::string>& v);

	private:
	
		template <class T> void writetype(int type, T value);
	
		template <class T> void write(const std::string& name, int type, T value);
		template <class T> void write(const unsigned int index, int type, T value);

		template <class T> void writeVector(const std::string& tableName, int type, std::vector<T> &v);
		template <class T> void writeVector(const unsigned int index, int type, std::vector<T> &v);
		
		/// write the currently nest table paths
		void writeTablePath();
	
		struct TableIndex {
			int type;           //< LUA_TSTRING or LUA_TNUMBER
			std::string name;        //< name index
			unsigned int index; //< number index
			operator std::string() {
				if(type == LUA_TNUMBER) {
					return std::to_string(index);
				}
				return name;
			}
		};
		std::vector<TableIndex> tables; //< the currently open table stack
	
		bool bCommentBlock;  //< currently in a comment block?
		std::stringstream buffer; //< string buffer
};

// TEMPLATE FUNCTIONS

template <class T>
void ofxLuaFileWriter::writetype(int type, T value) {}

template <class T>
void ofxLuaFileWriter::write(const std::string& name, int type, T value) {
	if(tables.empty()) {
		buffer << name << " = ";
	}
	else {
		writeTablePath();
		buffer << "." << name << " = ";
	}
	writetype(type, value);
	buffer << std::endl;
}

template <class T>
void ofxLuaFileWriter::write(const unsigned int index, int type, T value) {
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write var to file by index, no open tables";
		return;
	}
	writeTablePath();
	buffer << "[" << index << "] = ";
	writetype(type, value);
	buffer << std::endl;
}

template <class T>
void ofxLuaFileWriter::writeVector(const std::string& tableName, int type, std::vector<T> &v) {
	
	if(v.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write empty vector to file";
		return;
	}
	
	// write name
	if(tables.empty()) {
		buffer << tableName << " = { ";
	}
	else {
		writeTablePath();
		buffer << "." << tableName << " = { ";
	}
	
	// write vector
	writetype(type, v[0]);
	for(size_t i = 1; i < v.size(); ++i) {
		buffer << ", ";
		writetype(type, v[i]);
	}
	buffer << " }" << std::endl;
}

template <class T>
void ofxLuaFileWriter::writeVector(const unsigned int index, int type, std::vector<T> &v) {
	
	if(tables.empty()) {
		ofLogWarning("ofxLua") << "Couldn't write vector to file by index, no open tables";
		return;
	}
	
	// write name
	writeTablePath();
	buffer << "[" << index << "] = { ";
	
	// write vector
	writetype(type, v[0]);
	for(size_t i = 1; i < v.size(); ++i) {
		buffer << ", ";
		writetype(type, v[i]);
	}
	buffer << " }" << std::endl;
}

