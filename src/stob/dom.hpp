/* The MIT License:

Copyright (c) 2012 Ivan Gagis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

#pragma once


#include <string>

#include <ting/PoolStored.hpp>
#include <ting/Ptr.hpp>
#include <ting/fs/File.hpp>

#include "Exc.hpp"



namespace stob{



class Node{
	std::string value; //node value
	
	ting::Ptr<Node> next; //next sibling node
	
	ting::Ptr<Node> children; //pointer to the first child
	
	//constructor is private, no inheritance.
	Node(){}
	
	static void* operator new(size_t size);
		
public:
	static void operator delete(void *p)throw();
	
	static inline ting::Ptr<Node> New(){
		return ting::Ptr<Node>(new Node());
	}
	
	inline const std::string& Value()const throw(){
		return this->value;
	}
	
	inline void SetValue(const std::string& value)throw(){
		this->value = value;
	}
	
	//TODO: as int, as uint, as float, as double, as boolean
	
	void SetChildren(const ting::Ptr<Node>& first)throw(){
		this->children = first;
	}
	
	inline ting::Ptr<Node> ExtractChildren()throw(){
		return this->children;
	}
	
	inline Node* Child()throw(){
		return this->children.operator->();
	}
	
	//TODO: get children by name etc.
	
	//TODO: insert after/before
	
	//TODO: pull out (remove)
	
	void Write(ting::fs::File& fi);
};



ting::Ptr<Node> Load(ting::fs::File& fi);



}//~namespace