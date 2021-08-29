#pragma once

#ifndef _CALCULATOR_H_

#define _CALCULATOR_H_

#include <vector>
#include <string>
#include <sstream>

_STD_BEGIN

vector<string> operators({ "+", "-", "*", "/", "^", "(", ")", "sin", "cos", "tan", "log", "log10" });
vector<string> binary_op({ "+", "-", "*", "/", "^" });
vector<string> empties({ " ", "\t", "\n" });

// get vec[start, end)
template <typename Type>
vector<Type> subvector(vector<Type> vec, size_t start, size_t end)
{
	vec.erase(vec.begin(), vec.begin() + start);
	vec.erase(vec.begin() + end - start, vec.end());
	return vec;
}

double to_double(const string& str)
{
	stringstream ss;
	double result;
	ss << str;
	ss >> result;
	return result;
}
void error(string message) { cout << message << endl; exit(1); }
// all coincident
template <typename Type>
bool vector_include(vector<Type> v, Type t)
{
	for (size_t i = 0; i < v.size(); i++)
	{
		if (t == v[i]) return true;
	}
	return false;
}
// return the num
template <typename Type>
size_t vector_finds(vector<Type> v, Type t)
{
	size_t num = 0;
	for (size_t i = 0; i < v.size(); ++i)
	{
		if (v[i] == t) ++num;
	}
	return num;
}
// return the index
size_t first_str(string str, string find)
{
	if (find.length() > str.length()) return false;
	else if (find.length() == str.length()) return str == find;
	for (size_t i = 0; i <= str.length() - find.length(); ++i)
	{
		if (str.substr(i, find.length()) == find) return i;
	}
	return -1;
}

void replace(string& str, size_t _off, size_t _count, string to_)
{
	str = str.substr(0, _off) + to_ + str.substr(_off + _count + 1, str.length() - _off - _count - 1);
}

void change_all(initializer_list<char> ch_old, initializer_list<char> ch_new, string& str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		size_t j = 0;
		for (char ch : ch_old)
		{
			if (str[i] == ch)
			{
				str[i] = *(ch_new.begin() + j);
				break;
			}
			++j;
		}
	}
}

bool isnum(char ch)
{
	if ('0' <= ch && ch <= '9') return true;
	else if (ch == '.') return true;
	else return false;
}
bool isnum(string str)
{
	size_t num_of_dots = 0;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!isnum(str[i])) return false;
		if (str[i] == '.') num_of_dots++;
	}
	if (num_of_dots > 1) return false;
	return true;
}

vector<string> parse(string equation)
{
	vector<string> stack;
	string temp = "";
	for (size_t i = 0; i < equation.length(); ++i)
	{
		if (isnum(equation[i])) temp += equation[i];
		else if (vector_include(empties, static_cast<string>("" + equation[i])))
		{
			if (temp != "")
			{
				stack.push_back(temp);
				temp = "";
			}
			continue;
		}
		else if (vector_include(operators, equation.substr(i, 1)))
		{
			stack.push_back(temp);
			stack.push_back(equation.substr(i, 1));
			temp = "";
		}
		else if (vector_include(operators, equation.substr(i, 3)))
		{
			stack.push_back(temp);
			stack.push_back(equation.substr(i, 3));
			temp = "";
			i += 2;
		}
		else if (vector_include(operators, equation.substr(i, 5)))
		{
			stack.push_back(temp);
			stack.push_back(equation.substr(i, 5));
			temp = "";
			i += 4;
		}
		else error("Invalid charactor: " + equation[i]);
	}
	if (temp != "") stack.push_back(temp);
	return stack;
}

double no_brackets_calculate(vector<string> stack)
{
	if (stack.size() == 1 && isnum(stack[0])) return to_double(stack[0]);

	// sin cos tan log log10
	size_t num_of_powers = 0;
	size_t power_begin; // the index
	size_t power_end; // the index
	bool in_power = false;
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (in_power)
		{
			if (isnum(stack[i]))
			{
				num_of_powers--;
				if (num_of_powers == 0)
				{
					power_end = i;
					vector<string>::iterator stackitbegin = stack.begin() + power_begin;
					vector<string>::iterator stackitend = stack.begin() + power_end + 1;
					double power = no_brackets_calculate(subvector(stack, power_begin, power_end));
					if (power_end == stack.size() - 1) error("No number after \"" + stack[power_end - 2] + stack[power_end - 1] + stack[power_end] + "\".");
					else if (!isnum(stack[power_end + 1])) error("No number after \"" + stack[power_end - 2] + stack[power_end - 1] + stack[power_end] + "\".");
					string tp = stack[power_begin - 1];
					if (tp == "sin")
						stack[power_begin - 1] = to_string(pow(sin(to_double(stack[power_end + 1])), power));
					else if (tp == "cos")
						stack[power_begin - 1] = to_string(pow(cos(to_double(stack[power_end + 1])), power));
					else if (tp == "tan")
						stack[power_begin - 1] = to_string(pow(tan(to_double(stack[power_end + 1])), power));
					else if (tp == "log")
						stack[power_begin - 1] = to_string(pow(log(to_double(stack[power_end + 1])), power));
					else // log10
						stack[power_begin - 1] = to_string(pow(log10(to_double(stack[power_end + 1])), power));
					stack.erase(stackitbegin, stackitend + 1);
					i = power_begin - 1;
					in_power = false;
				}
			}
		}
		else if (stack[i] == "sin" || stack[i] == "cos" || stack[i] == "tan" ||
			stack[i] == "log" || stack[i] == "log10")
		{
			if (i == stack.size() - 1) error("No number after \"" + stack[i] + "\".");
			else if (stack[i + 1] == "^") // as sin^2 3.14159
			{
				in_power = true;
				num_of_powers++;
				if (num_of_powers == 1)
					power_begin = i;
			}
			else if (isnum(stack[i + 1])) // as sin 3.14159
			{
				string tp = stack[i];
				if (tp == "sin")
					stack[i] = to_string(sin(to_double(stack[i + 1])));
				else if (tp == "cos")
					stack[i] = to_string(cos(to_double(stack[i + 1])));
				else if (tp == "tan")
					stack[i] = to_string(tan(to_double(stack[i + 1])));
				else if (tp == "log")
					stack[i] = to_string(log(to_double(stack[i + 1])));
				else // log10
					stack[i] = to_string(log10(to_double(stack[i + 1])));
				--i;
			}
		}
	}

	// ^
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (stack[i] == "^")
		{
			if (i == stack.size() - 1) error("No number after \"^\".");
			else if (i == 0) error("No number before \"^\".");
			else if (!isnum(stack[i + 1])) error("\"" + stack[i + 1] + "\" after \"^\".");
			else if (!isnum(stack[i - 1])) error("\"" + stack[i - 1] + "\" before \"^\".");
			else
			{
				vector<string>::iterator it = stack.begin() + i;
				double power = to_double(stack[i + 1]);
				stack.erase(it, it + 2);
				stack[i - 1] = to_string(pow(to_double(stack[i - 1]), power));
				--i;
			}
		}
	}

	// * /
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (stack[i] == "*" || stack[i] == "/")
		{
			if (i == stack.size() - 1) error("No number after \"" + stack[i] + "\".");
			else if (i == 0) error("No number before \"" + stack[i] + "\".");
			else if (!isnum(stack[i + 1])) error("\"" + stack[i + 1] + "\" after \"" + stack[i] + "\".");
			else if (!isnum(stack[i - 1])) error("\"" + stack[i - 1] + "\" before \"" + stack[i] + "\".");
			else
			{
				vector<string>::iterator it = stack.begin() + i;
				double num1 = to_double(stack[i - 1]);
				double num2 = to_double(stack[i + 1]);
				stack[i - 1] = to_string(stack[i] == "*" ? num1 * num2 : num1 / num2);
				stack.erase(it, it + 2);
				--i;
			}
		}
	}

	// + -
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (stack[i] == "+" || stack[i] == "-")
		{
			if (i == stack.size() - 1) error("No number after \"" + stack[i] + "\".");
			else if (i == 0) error("No number before \"" + stack[i] + "\".");
			else if (!isnum(stack[i + 1])) error("\"" + stack[i + 1] + "\" after \"" + stack[i] + "\".");
			else if (!isnum(stack[i - 1])) error("\"" + stack[i - 1] + "\" before \"" + stack[i] + "\".");
			else
			{
				vector<string>::iterator it = stack.begin() + i;
				double num1 = to_double(stack[i - 1]);
				double num2 = to_double(stack[i + 1]);
				stack[i - 1] = to_string(stack[i] == "+" ? num1 + num2 : num1 - num2);
				stack.erase(it, it + 2);
				--i;
			}
		}
	}

	return to_double(stack[0]);
}

double no_brackets_calculate(string equation)
{
	vector<string> stack = parse(equation);
	/*for (size_t t = 0; t < equation.length(); ++t)
	{
		if (isnum(equation[t])) temp += equation[t];
		else if (vector_include(operators, "" + equation[t]) ||
			vector_include(operators, equation.substr(t, 3)) ||
			vector_include(operators, equation.substr(t, 5)))
		{
			if (vector_include(binary_op, "" + equation[t]))
			{
				if (equation[t] == '^')
				{
					string next_num = "";
					size_t j;
					for (j = t + 1; j < equation.length(); ++j)
					{
						if (vector_include(binary_op, "" + equation[j])) break;
						else if (vector_include(empties, "" + equation[j])) continue;
						else if (isnum(equation[j])) next_num += equation[j];
						else
						{
							error("Invalid charactor: " + equation[j]);
							goto BREAK;
						}
					}
					if (next_num == "")
					{
						error("No numbers between operator ^ and " + equation[j]);
						break;
					}
				}
				else if (equation[t] == '*' || equation[t] == '/')
				{

				}
			}

			if (vector_include(operators, equation.substr(t, 5))) t += 4;
			else if (vector_include(operators, equation.substr(t, 3))) t += 2;
		}
	}
BREAK:*/
	return no_brackets_calculate(stack);
}

double calculate(vector<string> stack)
{
	size_t lefts = vector_finds(stack, static_cast<string>("("));
	size_t rights = vector_finds(stack, static_cast<string>(")"));
	size_t layer = 0;
	size_t layer_begin; // index
	size_t layer_end; // index
	if (lefts != rights)
	{
		if (lefts < rights) error(to_string(rights - lefts) + " right bracket(s) are missing at the end.");
		else error(to_string(lefts - rights) + " left bracket(s) are missing at the beginning.");
	}
	if (vector_include(stack, static_cast<string>("("))) return no_brackets_calculate(stack);
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (stack[i] == "(") //++layer, layer_begin = i;
		{
			++layer;
			if (layer == 1) layer_begin = i;
			if (i != 0)
			{
				if (isnum(stack[i - 1]) || stack[i - 1] == ")")
				{
					stack.insert(stack.begin() + i, "*");
					return calculate(stack);
				}
			}
		}
		if (stack[i] == ")")
		{
			--layer;
			if (layer == 0)
			{
				layer_end = i;
				double result = calculate(subvector(stack, layer_begin + 1, layer_end));
				stack.erase(stack.begin() + layer_begin + 1, stack.begin() + layer_end + 1);
				stack[layer_begin] = to_string(result);
				i = layer_begin + 1;
			}
		}
	}
	return no_brackets_calculate(stack);
}
double calculate(string equation)
{
	vector<string> stack = parse(equation);
	return calculate(stack);
}

_STD_END

#endif