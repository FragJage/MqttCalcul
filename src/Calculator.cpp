#include "Calculator.h"
#include "MqttBase\MqttDaemon.h"

using namespace std;

SimpleLog* Calculator::m_Log = nullptr;

void Calculator::SetLogger(SimpleLog* pLog)
{
	m_Log = pLog;
}

string Calculator::Evaluate(CalculData* pCaclul, const map<string, string>& values)
{
	string formula;
	double value;


	if(m_Log) LOG_ENTER;
	if (m_Log) LOG_VERBOSE(m_Log) << "Evaluate " << pCaclul->GetName() << " => " << pCaclul->GetFormula();

	formula = VariablesReplace(pCaclul, values);
	if (formula == "") return "";
	if (m_Log) LOG_VERBOSE(m_Log) << "Translated formula " << formula;

	value = EvaluateOperation(formula.c_str(), NULL, NULL, 0, NULL);
	if (m_Log) LOG_VERBOSE(m_Log) << " -> value = " << value;

	ostringstream oss;
	oss << value;

	if (m_Log) LOG_EXIT_OK;
	return oss.str();
}

string Calculator::VariablesReplace(CalculData* pCaclul, const map<string, string>& values)
{
	ostringstream oss;
	string formula;
	size_t pos;
	vector<CalculData::Device> devices;
	map<string, string>::const_iterator itValues;


	pos = 0;
	devices = pCaclul->GetDevices();
	formula = pCaclul->GetFormula();

	for (auto itParse = devices.begin(); itParse != devices.end(); ++itParse)
	{
		itValues = values.find(itParse->Server + ":" + itParse->Topic);
		if (itValues == values.cend())
		{
			if (m_Log) LOG_INFO(m_Log) << "Unable to evaluate " << pCaclul->GetName() << ", device " << itParse->Server + ":" + itParse->Topic << " is not in the cache";
			return "";
		}
		oss << formula.substr(pos, itParse->Start - pos);
		oss << itValues->second;
		pos = itParse->End + 1;
	}

	if (pos<formula.size()) oss << formula.substr(pos);

	return oss.str();
}

double Calculator::EvaluateOperation(const char *calcul, double *G_dl, char *Op_dl, unsigned int prio_op_pre, const char **p_calcul)
{
	double G, D;                //opérande de gauche et opérande de droite
	char op, op_suivant;

	// initialisations
	// Si G_dl est un pointeur valide
	if (G_dl != NULL)
	{
		G = *G_dl;
	}
	else
	{                            // Sinon on lit G
		if (*calcul == '(')
			G = EvaluateOperation(calcul + 1, NULL, NULL, 0, &calcul);
		else
			G = strtod(calcul, (char **)&calcul);
	}

	// Si Op_dl est un pointeur valide
	if (Op_dl != NULL)
	{
		op = *Op_dl;
	}
	else
	{                            // Sinon on lit l'opérateur
		op = *calcul;
		calcul++;
	}

	// boucle des calculs
	while (op != '\0' && op != ')' && GetPriority(op) > prio_op_pre)
	{
		// Lecture de l'opérande de droite
		if (*calcul == '(')
			D = EvaluateOperation(calcul + 1, NULL, NULL, 0, &calcul);
		else
			D = strtod(calcul, (char **)&calcul);

		// Opérateur suivant
		op_suivant = *calcul;
		calcul++;

		if (IsOperation(op_suivant) && GetPriority(op_suivant) > GetPriority(op))
		{
			D = EvaluateOperation(calcul, &D, &op_suivant, GetPriority(op), &calcul);
		}

		G = DoOperation(G, op, D);
		op = op_suivant;
	}

	// Mise à jour de l'opérateur suivant pour la fonction appelante
	if (Op_dl != NULL)
		*Op_dl = op_suivant;

	// A pour effet d'indiquer à la fonction appelante jusqu'où la fonction appelée a lu la chaine 'calcul'
	if (p_calcul != NULL)
		*p_calcul = calcul;

	return G;
}

unsigned int Calculator::GetPriority(char op)
{
	unsigned int prio = 0;

	switch (op)
	{
		case '+':
		case '-':
			prio = 1;
			break;
		case '*':
		case '/':
			prio = 2;
			break;
		case '|':
		case '&':
		case '>':
		case '<':
			prio = 3;
			break;
	}

	return prio;
}

int Calculator::IsOperation(char op)
{
	switch (op)
	{
		case '+':
		case '-':
		case '*':
		case '/':
		case '|':
		case '&':
		case '>':
		case '<':
			return 1;
		default:
			return 0;
	}
}

double Calculator::DoOperation(double G, char op, double D)
{
	double resultat = 0;

	switch (op)
	{
		case '+':
			resultat = G + D;
			break;
		case '-':
			resultat = G - D;
			break;
		case '*':
			resultat = G * D;
			break;
		case '/':
			resultat = G / D;
			break;
		case '|':
			resultat = (int)G | (int)D;
			break;
		case '&':
			resultat = (int)G & (int)D;
			break;
		case '<':
			if (G < D)
				return 1;
			else
				return 0;
			break;
		case '>':
			if (G > D)
				return 1;
			else
				return 0;
			break;
		default:
			resultat = 0;
	}

	return resultat;
}
