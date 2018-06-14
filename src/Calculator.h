#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <map>
#include "SimpleLog/SimpleLog.h"
#include "CalculData.h"

class Calculator
{
    public:
		static std::string Evaluate(CalculData* pCaclul, const std::map<std::string, std::string>& values);
		static void SetLogger(SimpleLog* pLog);

	private:
		static std::string VariablesReplace(CalculData* pCaclul, const std::map<std::string, std::string>& values);
		static double EvaluateOperation(const char *calcul, double *G_dl, char *Op_dl, unsigned int prio_op_pre, const char **p_calcul);
		static int IsOperation(char op);
		static double DoOperation(double G, char op, double D);
		static unsigned int GetPriority(char op);
		static SimpleLog* m_Log;
};

#endif // CALCULATOR_H
