#pragma once
#include "stdafx.h"

#define MFST_DIAGN_MAXSIZE 2*ERROR_MAXSIZE_MESSAGE
#define MFST_DIAGN_NUMBER 3
#define MFST_TRACE_START	std::cout << std::setfill(' ')  << std::setw(4)<< std::left << "���" << ": " \
									  << std::setw(20) << std::left << "�������" \
									  << std::setw(30) << std::left << "������� �����" \
									  << std::setw(20) << std::left << "����" \
									  << std::endl;

#define MFST_TRACE1		std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " \
								  << std::setw(20) << std::left << rule.getCRule(rbuf, nrulechain) \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE2		std::cout << std::setw(4) << std::left << FST_TRACE_n<<": " \
								  << std::setw(20) << std::left << " " \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE3		std::cout << std::setw(4) << std::left << ++FST_TRACE_n <<": " \
								  << std::setw(20) << std::left << " " \
								  << std::setw(30) << std::left << getCLenta(lbuf, lenta_position) \
								  << std::setw(20) << std::left << getCSt(sbuf) \
								  << std::endl;

#define MFST_TRACE4(c)	std::cout << std::setw(4) << std::left << ++FST_TRACE_n << ": " << std::setw(20) << std::left << c <<std::endl; 
#define MFST_TRACE5(c)	std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << std::endl; 
#define MFST_TRACE6(c, k)	std::cout << std::setw(4) << std::left << FST_TRACE_n << ": " << std::setw(20) << std::left << c << k << std::endl; 
#define MFST_TRACE7		std::cout << std::setw(4) << std::left << state.lenta_position << ": " \
								  << std::setw(20) << std::left << rule.getCRule(rbuf, state.nrulechain) \
								  << std::endl;

typedef std::stack<short> MFSTSTACK;		// ���� ��������

namespace MFST
{
	struct MfstState				// ��������� �������� (��� ����������)
	{
		short lenta_position;			// ������� �� �����
		short nrule;					// ����� �������� �������
		short nrulechain;				// ����� ������� �������, �������� �������
		MFSTSTACK st;					// ���� ��������
		MfstState();
		MfstState(
			short pposition,			// ������� �� �����
			MFSTSTACK pst,				// ���� ��������
			short pnrulechain			// ����� ������� �������, �������� �������
		);
		MfstState(
			short pposition,			// ������� �� �����
			MFSTSTACK pst,				// ���� ��������
			short pnrule,				// ����� �������� �������
			short pnrulechain			// ����� ������� �������, �������� �������
		);
	};

	struct Mfst				// ���������� �������
	{
		enum RC_STEP {			// ��� �������� ������� step
			NS_OK,				// ������� ������� � �������, ������� �������� � ����
			NS_NORULE,			// �� ������� ������� ���������� (������ � ����������)
			NS_NORULECHAIN,		// �� ������� ���������� ������� ������� (������ � �������� ����)
			NS_ERROR,			// ����������� �������������� ������ ����������
			TS_OK,				// ���. ������ ����� == ������� �����, ������������ �����, pop �����
			TS_NOK,				// ���. ������ ����� != ������� �����, ������������� ���������
			LENTA_END,			// ������� ������� ����� >= lenta_size
			SURPRISE			// ����������� ��� �������� (������ � step)
		};

		struct MfstDiagnosis	// �����������
		{
			short lenta_position;		// ������� �� �����
			RC_STEP rc_step;			// ��� ���������� ����
			short nrule;				// ����� �������
			short nrule_chain;			// ����� ������� �������
			MfstDiagnosis();
			MfstDiagnosis(
				short plenta_position,	// ������� �� �����
				RC_STEP prt_step,		// ��� ���������� ����
				short pnrule,			// ����� �������
				short pnrule_chain		// ����� ������� �������
			);
		} diagnosis[MFST_DIAGN_NUMBER];		// ��������� ����� �������� ���������

		GRBALPHABET* lenta;				// ���������������� (TS/NS) ����� (�� LEX)
		short lenta_position;			// ������� ������� �� �����
		short nrule;					// ����� �������� �������
		short nrulechain;				// ����� ������� �������, �������� �������
		short lenta_size;				// ������ �����
		GRB::Greibach grebach;			// ���������� �������
		Lex::LEX lex;					// ��������� ������ ������������ �����������
		MFSTSTACK st;					// ���� ��������
		std::stack<MfstState> storestate;	// ���� ��� ���������� ��������� 
		Mfst();
		Mfst(
			Lex::LEX plex,				// ��������� ������ ������������ �����������
			GRB::Greibach pgrebach,		// ���������� �������
			bool traceIsOnSwitch		// ���� ��� ���������� �����������
		);
		char* getCSt(char* buf);		// �������� ���������� �����
		char* getCLenta(char* buf, short pos, short n = 25);	// �����: n �������� � pos
		char* getDiagnosis(short n, char* buf);		// �������� n-� ������ ����������� ��� 0�00
		bool savestate();				// ��������� ��������� ��������
		bool reststate();				// ������������ ��������� ��������
		bool push_chain(				// ��������� ������� ������� � ����
			GRB::Rule::Chain chain		// ������� �������
		);
		RC_STEP step();					// ��������� ��� ��������
		bool traceIsOn;
		bool start();					// ��������� �������
		bool savediagnosis(
			RC_STEP pprc_step			// ��� ���������� ����
		);
		void printrules();				// ������� ������������������ ������

		struct Deducation		// �����
		{
			short size;			// ���������� ����� � ������
			short* nrules;		// ������ ������ ����������
			short* nrulechains;	// ������ ������� ������ ���������� (nrules)
			Deducation() { size = 0; nrules = 0; nrulechains = 0; };
		} deducation;

		bool savededucation();	// ��������� ������ ������
	};
}