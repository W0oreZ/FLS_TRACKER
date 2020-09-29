#ifndef StringSplitter_h
#define StringSplitter_h

#include "Arduino.h"

class StringSplitter{
	private:
		static const int MAX = 12;
		String op[MAX];
		int count = 0;
		int countOccurencesOfChar(String s, char c);

	public:
		StringSplitter(String s, char c, int l);		
		int getItemCount();
		String getItemAtIndex(int index);
};

#endif