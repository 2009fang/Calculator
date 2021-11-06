#pragma once

#ifndef _CALCULATOR_H_

#define _CALCULATOR_H_

#include <vector>
#include <string>
#include <sstream>
#include <corecrt_math_defines.h>

_STD_BEGIN

vector<string> operators({ "+", "-", "*", "/", "^", "(", ")", "sin", "cos", "tan", "log", "log10", "asin", "acos", "atan", "sqrt", "cubr" });
vector<string> constants({ "pi", "phi", "e" });
vector<string> binary_op({ "+", "-", "*", "/", "^" });
vector<string> empties({ " ", "\t", "\n" });

// get vec[start, end)
template <typename Type>
vector<Type> subvector(vector<Type> vec, size_t start, size_t end)
{
	vec.erase(vec.begin(), vec.begin() + start);
	vec.erase(vec.begin() + (end - start), vec.end());
	return vec;
}
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
double to_double(const string& str)
{
	if (str == "pi") return acos(-1);
	if (str == "phi") return (1 + sqrt(5)) / 2;
	if (str == "e") return M_E;
	if (str == "-pi") return -acos(-1);
	if (str == "-phi") return -(1 + sqrt(5)) / 2;
	if (str == "-e") return -M_E;
	stringstream ss;
	double result;
	ss << str;
	ss >> result;
	return result;
}
void error(string message) { cout << message << endl; exit(1); }
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
bool isnum(char ch)
{
	if ('0' <= ch && ch <= '9') return true;
	else if (ch == '.') return true;
	else return false;
}
bool isnum(string str, bool constants_ = true)
{
	if (str == "-") return false;
	if (str[0] == '-') return isnum(str.substr(1, str.length() - 1));
	if (constants_ && vector_include(constants, str)) return true;
	size_t num_of_dots = 0;
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (!isnum(str[i])) return false;
		if (str[i] == '.') num_of_dots++;
	}
	if (num_of_dots > 1) return false;
	return true;
}

double calculate_powers(const vector<double>& power)
{
	double result = 1;
	for (size_t i = power.size() - 1; i >= 0 && i <= power.size(); --i)
	{
		result = pow(power[i], result);
	}
	return result;
}

vector<string> parse(string equation)
{
	string temp = "";
	vector<string> stack;
	for (size_t i = 0; i < equation.length(); ++i)
	{
		if (isnum(equation[i])) temp += equation[i];
		else if (vector_include(empties, equation.substr(i, 1)))
		{
			if (temp != "")
			{
				stack.push_back(temp);
				temp = "";
			}
		}
		else if (vector_include(operators, equation.substr(i, 1)))
		{
			if (equation[i] == '-')
			{
				if (i == 0) temp = "-";
				else if (!isnum(temp)) stack.push_back(temp), temp = "-";
			}
			if (temp == "-" && equation[i] == '-') continue;
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 1));
			temp = "";
		}
		else if (vector_include(operators, equation.substr(i, 3)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 3));
			temp = "";
			i += 2;
		}
		else if (vector_include(operators, equation.substr(i, 4)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 4));
			temp = "";
			i += 3;
		}
		else if (vector_include(operators, equation.substr(i, 5)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 5));
			temp = "";
			i += 4;
		}
		else if (vector_include(constants, equation.substr(i, 1)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 1));
			temp = "";
		}
		else if (vector_include(constants, equation.substr(i, 2)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 2));
			temp = "";
			i += 1;
		}
		else if (vector_include(constants, equation.substr(i, 3)))
		{
			if (temp != "") stack.push_back(temp);
			stack.push_back(equation.substr(i, 3));
			temp = "";
			i += 2;
		}
		else error("Invalid charactor: " + equation[i]);
	}
	if (temp != "") stack.push_back(temp);
	return stack;
}

double no_brackets_calculate(vector<string> stack)
{
	if (stack.size() == 1 && isnum(stack[0])) return to_double(stack[0]);

	// sin asin cos acos tan atan log log10 sqr cubr
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
					double power = no_brackets_calculate(subvector(stack, power_begin + 1, power_end + 1));
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
					else if (tp == "log10")
						stack[power_begin - 1] = to_string(pow(log10(to_double(stack[power_end + 1])), power));
					else if (tp == "asin")
						stack[power_begin - 1] = to_string(pow(asin(to_double(stack[power_end + 1])), power));
					else if (tp == "acos")
						stack[power_begin - 1] = to_string(pow(acos(to_double(stack[power_end + 1])), power));
					else if (tp == "atan")
						stack[power_begin - 1] = to_string(pow(atan(to_double(stack[power_end + 1])), power));
					else if (tp == "sqrt")
						stack[power_begin - 1] = to_string(pow(sqrt(to_double(stack[power_end + 1])), power));
					else if (tp == "cubr")
						stack[power_begin - 1] = to_string(pow(to_double(stack[power_end + 1]), power / 3));
					stack.erase(stackitbegin, stackitend + 1);
					i = power_begin - 1;
					if (power_begin != 1)
						if (isnum(stack[power_begin - 2]) || stack[power_begin - 2] == ")")
							stack.insert(stack.begin() + (power_begin - 1), "*"); // as 2sin^2 (pi/2) -> 2*sin^2 (pi/2)
					in_power = false;
				}
			}
		}
		else if (vector_include(operators, stack[i]) && !vector_include(binary_op, stack[i]) && stack[i] != "(" && stack[i] != ")")
		{
			if (i == stack.size() - 1) error("No number after \"" + stack[i] + "\".");
			else if (stack[i + 1] == "^") // as sin^2 3.14159
			{
				in_power = true;
				num_of_powers++;
				if (num_of_powers == 1)
					power_begin = i + 1;
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
				else if (tp == "log10")
					stack[i] = to_string(log10(to_double(stack[i + 1])));
				else if (tp == "asin")
					stack[i] = to_string(asin(to_double(stack[i + 1])));
				else if (tp == "acos")
					stack[i] = to_string(acos(to_double(stack[i + 1])));
				else if (tp == "atan")
					stack[i] = to_string(atan(to_double(stack[i + 1])));
				else if (tp == "sqrt")
					stack[i] = to_string(sqrt(to_double(stack[i + 1])));
				else if (tp == "cubr")
					stack[i] = to_string(pow(to_double(stack[i + 1]), 1.0 / 3.0));
				stack.erase(stack.begin() + i + 1);
				--i;
			}
		}
	}

	// ^
	size_t power_begin_po;
	bool power_begins = false;
	vector<double> powers;
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (power_begins)
		{
			if (isnum(stack[i])) powers.push_back(to_double(stack[i]));
			// if (stack[i] != "^" && !isnum(stack[i]))
			if (i == stack.size() - 1)
			{
				power_begins = false;
				vector<string>::iterator it = stack.begin() + power_begin_po;
				double power = calculate_powers(powers); // 2^2^3 = 2^(2^3) not= (2^2)^3
				stack.erase(it, it + (2 * powers.size() - 2));
				stack[power_begin_po] = to_string(power);
				i = power_begin_po;
			}
			else if (stack[i + 1] != "^" && !isnum(stack[i + 1]))
			{
				power_begins = false;
				vector<string>::iterator it = stack.begin() + power_begin_po;
				double power = calculate_powers(powers); // 2^2^3 = 2^(2^3) not= (2^2)^3
				stack.erase(it, it + (2 * powers.size() - 2));
				stack[power_begin_po] = to_string(power);
				i = power_begin_po;
				powers.clear();
			}
		}
		else if (stack[i] == "^")
		{
			power_begin_po = i - 1;
			power_begins = true;
			if (i == stack.size() - 1) error("No number after \"^\".");
			else if (i == 0) error("No number before \"^\".");
			else if (!isnum(stack[i + 1])) error("\"" + stack[i + 1] + "\" after \"^\".");
			else if (!isnum(stack[i - 1])) error("\"" + stack[i - 1] + "\" before \"^\".");
			else powers.push_back(to_double(stack[i - 1]));
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
			else if (i == 0)
			{
				if (stack[i] == "+") error("No number before \"+\".");
				else
				{
					double result;
					bool have_result = false;
					for (size_t j = i; j < stack.size(); ++j)
					{
						if (isnum(stack[j]))
						{
							result = to_double(stack[j]);
							have_result = true;
							break;
						}
					}
					if (stack.size() == 1) error("No number after \"-\".");
					else if (!isnum(stack[1])) error("No number after \"-\".");
					else
					{
						stack[0] = to_string(-to_double(stack[1]));
						stack.erase(stack.begin() + 1);
					}
				}
			}
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

double no_brackets_calculate(string equation) { return no_brackets_calculate(parse(equation)); }

double calculate(vector<string> stack)
{
	size_t lefts = vector_finds(stack, static_cast<string>("("));
	size_t rights = vector_finds(stack, static_cast<string>(")"));
	size_t layer = 0;
	size_t layer_begin; // index
	size_t layer_end; // index
	if (lefts != rights)
	{
		if (lefts < rights) error(to_string(rights - lefts) + " right bracket(s) is(are) missing at the end.");
		else error(to_string(lefts - rights) + " left bracket(s) is(are) missing at the beginning.");
	}
	if (!vector_include(stack, static_cast<string>("("))) return no_brackets_calculate(stack);
	for (size_t i = 0; i < stack.size(); ++i)
	{
		if (stack[i] == "(")
		{
			++layer;
			if (layer == 1) layer_begin = i;
			if (i != 0)
			{
				if (isnum(stack[i - 1])) // ...52(3+1)
				{
					if (i != 1)
					{
						if (stack[i - 2] != "^") // not ...sin^2(pi) or ...2^3(4+1)
						{
							stack.insert(stack.begin() + i, "*"); // ...5+2(2+1)
						}
						else // ...sin^2(pi) or ...2^3(4+1)
						{
							if (i != 2)
							{
								if (isnum(stack[i - 3]))
								{
									size_t brackets = 0, pos;
									for (pos = i; pos < stack.size(); ++pos)
									{
										if (stack[pos] == "(") brackets++;
										if (stack[pos] == ")") brackets--;
										if (brackets == 0) break;
									}
									double result = calculate(subvector(stack, i + 1, pos));
									double num = to_double(stack[i - 3]);
									stack.erase(stack.begin() + i, stack.begin() + pos + 1);
									stack[i - 1] = to_string(result * num);
									i -= 2;
									layer--;
									if (layer == 0) layer_end = pos;
								}
								else if (vector_include(operators, stack[i - 3]) && !vector_include(binary_op, stack[i - 3]) && (stack[i - 3] != "(" || stack[i - 3] != ")"))
								{
									size_t brackets = 0, pos;
									for (pos = i; pos < stack.size(); ++pos)
									{
										if (stack[pos] == "(") brackets++;
										if (stack[pos] == ")") brackets--;
										if (brackets == 0) break;
									}
									double result = calculate(subvector(stack, i + 1, pos));
									double num = to_double(stack[i - 3]);
									stack.erase(stack.begin() + i, stack.begin() + pos + 1);
									stack[i - 1] = to_string(result * num);
									i -= 2;
									layer--;
									if (layer == 0) layer_end = pos;
								}
							}
						}
					}
					else
					{
						stack.insert(stack.begin() + 1, "*");
						++i;
						++layer_begin;
					}
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
				i = layer_begin;
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