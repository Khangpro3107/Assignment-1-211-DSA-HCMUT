#include "SymbolTable.h"

void SymbolTable::run(string filename)
{
	ifstream fname;
	fname.open(filename);
	string cmd;
	bool just_print = true;
	int loop_count = 0;
	int current_scope = 0;
	while (getline(fname, cmd))
	{
		string arr[4];
		arr[0] = "";
		arr[1] = "";
		arr[2] = "";
		arr[3] = "";
		if (cmd != "PRINT" && cmd != "RPRINT" && cmd != "BEGIN" && cmd != "END") just_print = false;
		if (split(cmd, arr))
		{
			if (loop_count != 0 && arr[0] != "BEGIN" && arr[0] != "END" && arr[0] != "PRINT" && arr[0] != "RPRINT") cout << endl;
			if (arr[0] == "INSERT")
			{
				if (!is_identifier_name(arr[1])) {
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				if (subtables.head->data.symbols.find_symbol_name_local(arr[1]))
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw Redeclared(cmd);
					return;
				}
				if (arr[2] == "string")
				{
					node<Symbol>* new_symbol = new node<Symbol>(Symbol(current_scope, arr[1], false, ""));
					subtables.head->data.symbols.push_front(new_symbol);
					cout << "success";
					loop_count++;
				}
				else if (arr[2] == "number")
				{
					node<Symbol>* new_symbol = new node<Symbol>(Symbol(current_scope, arr[1], true, ""));
					subtables.head->data.symbols.push_front(new_symbol);
					cout << "success";
					loop_count++;
				}
				else
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
			}
			else if (arr[0] == "ASSIGN")
			{
				if (!is_identifier_name(arr[1])) {
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				bool arr2_identifier = is_identifier_name(arr[2]);
				bool arr2_number_input = is_number_input(arr[2]);
				bool arr2_string_input = is_string_input(arr[2]);
				if (!arr2_number_input && !arr2_string_input && !arr2_identifier)
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				Symbol arr1 = lookup_return_symbol(arr[1]);

				if (is_null_symbol(arr1))
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw Undeclared(cmd);
					return;
				}
				if (arr[2] == "")
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				if (!arr2_identifier)
				{
					if (arr2_number_input)
					{
						if (arr1.type == 0) //is string
						{
							node<SubSymbolTable>* ite = this->subtables.head;
							while (ite)
							{
								ite->data.symbols.destroy();
								ite = ite->next;
							}
							subtables.destroy();
							if (ite) delete ite;
							ite = nullptr;
							throw TypeMismatch(cmd);
							return;
						}
						arr1.value = arr[2];
						cout << "success";
						loop_count++;
					}
					else if (arr2_string_input)
					{
						if (arr1.type == 1) //is number
						{
							node<SubSymbolTable>* ite = this->subtables.head;
							while (ite)
							{
								ite->data.symbols.destroy();
								ite = ite->next;
							}
							subtables.destroy();
							if (ite) delete ite;
							ite = nullptr;
							throw TypeMismatch(cmd);
							return;
						}
						arr1.value = arr[2];
						cout << "success";
						loop_count++;
					}
					else
					{
						node<SubSymbolTable>* ite = this->subtables.head;
						while (ite)
						{
							ite->data.symbols.destroy();
							ite = ite->next;
						}
						subtables.destroy();
						if (ite) delete ite;
						ite = nullptr;
						throw InvalidInstruction(cmd);
						return;
					}
				}
				else {
					Symbol arr2 = lookup_return_symbol(arr[2]);
					if (is_null_symbol(arr2))
					{
						node<SubSymbolTable>* ite = this->subtables.head;
						while (ite)
						{
							ite->data.symbols.destroy();
							ite = ite->next;
						}
						subtables.destroy();
						if (ite) delete ite;
						ite = nullptr;
						throw Undeclared(cmd);
						return;
					}
					if (arr1.type != arr2.type)
					{
						node<SubSymbolTable>* ite = this->subtables.head;
						while (ite)
						{
							ite->data.symbols.destroy();
							ite = ite->next;
						}
						subtables.destroy();
						if (ite) delete ite;
						ite = nullptr;
						throw TypeMismatch(cmd);
						return;
					}
					arr1.value = arr2.value;
					cout << "success";
					loop_count++;
				}
			}
			else if (arr[0] == "BEGIN")
			{
				if (!(arr[1] == ""))
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				current_scope++;
				node<SubSymbolTable>* new_sub_symbol_table = new node<SubSymbolTable>(SubSymbolTable(current_scope, LL<Symbol>()));
				subtables.push_front(new_sub_symbol_table);
			}
			else if (arr[0] == "END")
			{
				if (!(arr[1] == ""))
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				current_scope--;
				if (current_scope < 0)
				{
					if (loop_count != 0) cout << endl;
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw UnknownBlock();
					return;
				}
				this->subtables.head->data.symbols.destroy();
				this->subtables.pop_front();
			}
			else if (arr[0] == "LOOKUP")
			{
				if (!is_identifier_name(arr[1]) || arr[2] != "")
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				int res = lookup(arr[1]);
				if (res == -1)
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw Undeclared(cmd);
					return;
				}
				cout << res;
				loop_count++;
			}
			else if (arr[0] == "PRINT")
			{
				if (arr[1] != "")
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				string temp = to_print();
				if (temp != "")
				{
					cout << endl << temp;
				}
			}
			else if (arr[0] == "RPRINT")
			{
				if (arr[1] != "")
				{
					node<SubSymbolTable>* ite = this->subtables.head;
					while (ite)
					{
						ite->data.symbols.destroy();
						ite = ite->next;
					}
					subtables.destroy();
					if (ite) delete ite;
					ite = nullptr;
					throw InvalidInstruction(cmd);
					return;
				}
				string temp = to_rprint();
				if (temp != "")
				{
					cout << endl << temp;
				}
			}
			else
			{
				node<SubSymbolTable>* ite = this->subtables.head;
				while (ite)
				{
					ite->data.symbols.destroy();
					ite = ite->next;
				}
				subtables.destroy();
				if (ite) delete ite;
				ite = nullptr;
				throw InvalidInstruction(cmd);
				return;
			}

		}
		else
		{
			if (!just_print) cout << endl;
			node<SubSymbolTable>* ite = this->subtables.head;
			while (ite)
			{
				ite->data.symbols.destroy();
				ite = ite->next;
			}
			subtables.destroy();
			if (ite) delete ite;
			ite = nullptr;
			throw InvalidInstruction(cmd);
			return;
		}

	}
	if (current_scope)
	{
		if (loop_count != 0) cout << endl;
		node<SubSymbolTable>* ite = this->subtables.head;
		while (ite)
		{
			ite->data.symbols.destroy();
			ite = ite->next;
		}
		subtables.destroy();
		if (ite) delete ite;
		ite = nullptr;
		throw UnclosedBlock(current_scope);
		return;
	}
	if (!just_print) cout << endl;
	if (cmd != "PRINT" && cmd != "RPRINT" && cmd != "BEGIN" && cmd != "END") just_print = false;
	node<SubSymbolTable>* ite = this->subtables.head;
	while (ite)
	{
		ite->data.symbols.destroy();
		ite = ite->next;
	}
	subtables.destroy();
	if (ite) delete ite;
	ite = nullptr;
}


