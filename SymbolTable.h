#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

inline bool split(const string& str, string* result, const char& split_chr = ' ')
{
	int n = str.length();
	if (n == 0) return false;
	if (str[n - 1] == '\'')
	{
		int found1 = str.find('\'');
		if (found1 == n - 1) return false;
		result[2] = str.substr(found1, n - 1 - found1 + 1);
		int found_space = str.find(' ');
		result[0] = str.substr(0, found_space);
		result[1] = str.substr(found_space + 1, found1 - found_space - 2);
		return true;
	}
	else
	{
		int i = 0, j = 0;
		string temp = "";
		while (str[i] != '\0')
		{
			if (j > 2) return false;
			if (str[i] == split_chr)
			{
				result[j] = temp;
				temp = "";
				j++;
			}
			else
			{
				temp += str[i];
			}
			i++;
		}
		result[j] = temp;
		return true;
	}
}
inline bool is_cap(const char& c)
{
	if (c >= 'A' && c <= 'Z') return true;
	return false;
}
inline bool is_letter(const char& c)
{
	if (c >= 'a' && c <= 'z') return true;
	return false;
}
inline bool is_number(const char& c)
{
	if (c >= '0' && c <= '9') return true;
	return false;
}
inline bool is_identifier_name(const string& s)
{
	if (!(s[0] >= 'a' && s[0] <= 'z')) return false;
	for (int i = 1; s[i] != '\0'; i++)
	{
		if (!(s[i] == '_' || is_cap(s[i]) || is_letter(s[i]) || is_number(s[i]))) return false;
	}
	return true;
}
inline bool is_number_input(const string& s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		if (!is_number(s[i])) return false;
	}
	return true;
}
inline bool is_string_input(const string& s)
{
	int n = s.length();
	if (n < 2) return false;
	if (s[0] != '\'' || s[n - 1] != '\'') return false;
	for (int i = 1; i < n - 1; i++)
	{
		if (!(is_number(s[i]) || is_cap(s[i]) || is_letter(s[i]) || s[i] == ' ')) return false;
	}
	return true;
}

struct Symbol
{
	int scope_level;
	string name;
	bool type;
	string value;
	Symbol(const int& scope_level = 0, const string& name = "", const bool& type = 0, const string& value = "_null")
	{
		//type: string is 0, number is 1
		this->scope_level = scope_level;
		this->name = name;
		this->type = type;
		this->value = value;
	}
	void operator=(const Symbol& a)
	{
		this->name = a.name;
		this->scope_level = a.scope_level;
		this->type = a.type;
		this->value = a.value;
	}
	void operator=(const Symbol*& a)
	{
		this->name = a->name;
		this->scope_level = a->scope_level;
		this->type = a->type;
		this->value = a->value;
	}
};

inline bool is_null_symbol(const Symbol& s)
{
	if (s.value == "_null") return true;
	return false;
}

template<class T>
struct node
{
	T data;
	node<T>* next;
	node(const T& dat, node<T>* n = nullptr)
	{
		this->data = dat;
		this->next = n;
	}
	node(const T*& ptr)
	{
		this->next = nullptr;
		this->data = ptr;
	}
};

template <class T>
struct LL
{
	node<T>* head;
	node<T>* tail;
	int size;
	LL(node<T>* h = nullptr, node<T>* t = nullptr, const int& s = 0)
	{
		this->head = h;
		this->tail = t;
		this->size = s;
	}
	void push_front(node<T>* ptr)
	{
		if (!head)
		{
			head = ptr;
			tail = ptr;
			size = 1;
			return;
		}
		else
		{
			ptr->next = head;
			head = ptr;
			size++;
			return;
		}
	}
	void pop_front()
	{
		if (size == 0) return;
		if (size == 1)
		{
			node<T>* deleted = head;
			head = nullptr;
			tail = nullptr;
			delete deleted;
			size = 0;
			return;
		}
		node<T>* deleted = head;
		head = head->next;
		deleted->next = nullptr;
		delete deleted;
		size--;
	}
	bool find_symbol_name_local(const string& s)
	{
		if (size == 0) return false;
		node<T>* ite = head;
		while (ite)
		{
			if (ite->data.name == s) return true;
			ite = ite->next;
		}
		return false;
	}
	Symbol find_return_symbol_local(const string& s)
	{
		if (size == 0) return Symbol();	//nothing to look for
		node<T>* ite = head;
		while (ite)
		{
			if (ite->data.name == s) return ite->data;
			ite = ite->next;
		}
		return Symbol();	//symbol not found
	}
	void destroy()
	{
		if (this->size == 0) return;
		node<T>* prev = this->head;
		node<T>* cur = this->head->next;
		while (prev && cur)
		{
			delete prev;
			prev = cur;
			cur = cur->next;
		}
		delete prev;
		prev = NULL;
		size = 0;
	}
};

struct SubSymbolTable
{
	int scope;
	LL<Symbol> symbols;
	SubSymbolTable(const int& s = 0, const LL<Symbol>& l = LL<Symbol>())
	{
		this->scope = s;
		this->symbols.head = l.head;
		this->symbols.tail = l.tail;
		this->symbols.size = l.size;
	}
	void operator=(const SubSymbolTable*& a)
	{
		this->scope = a->scope;
		this->symbols.head = a->symbols.head;
		this->symbols.size = a->symbols.size;
		this->symbols.tail = a->symbols.tail;
	}
};

inline bool is_in(const string& s, const LL<string>& a)
{
	node<string>* ite = a.head;
	while (ite)
	{
		if (s == ite->data)
		{
			return true;
		}
		ite = ite->next;
	}
	return false;
}

class SymbolTable
{
public:
	LL<SubSymbolTable> subtables;
	SymbolTable()
	{
		this->subtables = LL<SubSymbolTable>();
		node<SubSymbolTable>* sub_symbol_table = new node<SubSymbolTable>(SubSymbolTable(0, LL<Symbol>()));
		subtables.push_front(sub_symbol_table);
	}
	inline void run(string filename);
	int lookup(string label)
	{
		//find the label, if found return its scope level, else return -1
		//outdated: node<SubSymbolTable>* ite = new node<SubSymbolTable>(this->subtables.head->data);
		node<SubSymbolTable>* ite = this->subtables.head;
		while (ite)
		{
			if (ite->data.symbols.find_symbol_name_local(label))
			{
				return ite->data.scope;
			}
			ite = ite->next;
		}
		return -1;

	}
	Symbol lookup_return_symbol(const string& label)
	{
		node<SubSymbolTable>* ite = this->subtables.head;
		while (ite)
		{
			Symbol ret = ite->data.symbols.find_return_symbol_local(label);
			if (!(is_null_symbol(ret)))
			{
				return ret;
			}
			ite = ite->next;
		}
		return Symbol();
	}
	bool is_defined(const string& s)
	{
		if (this->lookup(s) == -1) return false;
		return true;
	}
	string to_rprint()
	{
		LL<string> printing_track;
		string result = "";
		node<SubSymbolTable>* ite = this->subtables.head;
		int i = 0;
		while (ite)
		{
			node<Symbol>* ite2 = ite->data.symbols.head;
			while (ite2)
			{
				if (!is_in(ite2->data.name, printing_track))
				{
					node<string>* string_node = new node<string>(ite2->data.name, nullptr);
					printing_track.push_front(string_node);
					if (i != 0) result += " ";
					result += ite2->data.name;
					result += "//";
					result += to_string(ite2->data.scope_level);
				}
				ite2 = ite2->next;
				i++;
			}
			ite = ite->next;
		}
		printing_track.destroy();
		return result;
	}
	string to_print()
	{
		LL<string> printing_track;
		string result = "";
		node<SubSymbolTable>* ite = this->subtables.head;
		int i = 0;
		while (ite)
		{
			node<Symbol>* ite2 = ite->data.symbols.head;
			
			while (ite2)
			{
				if (!is_in(ite2->data.name, printing_track))
				{
					node<string>* string_node = new node<string>(ite2->data.name, nullptr);
					printing_track.push_front(string_node);
					if (i != 0) result = ite2->data.name + "//" + to_string(ite2->data.scope_level) + " " + result;
					else
					result = ite2->data.name + "//" + to_string(ite2->data.scope_level) + result;
				}
				ite2 = ite2->next;
				i++;
			}
			ite = ite->next;
		}
		printing_track.destroy();
		return result;
	}
};
#endif