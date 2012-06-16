
#include "trab.h"
#include "trab_cln.c"
#include <string.h>


int current_user, manager;

struct user_ip{
	char name[60];
	char ip[40];
	int keepAlive;
};

int
find_by_address(commomattributtes attr)
{
	int i;
	for(i=0;i<10;i++)
		if(strcmp(attr.address,list[i])==0)
			return i;
}

//fazer parse de um arquivo que contenha o endereco ip dos hosts
struct user_ip list[10];

int has_ask = 0;

form ask;

form answer[10][3];

control *
whatdoto_1_svc(control *argp, struct svc_req *rqstp)
{
	static control  result;
	if(current_user == manager)
	{
		result.action = 1;
	}
	else if(has_ask)
	{
		//result.attr.booleanVar = 1;
		result.action = 2;
	}
	else if(1)
	{
		result.action = 3;
	}
	else
	{
		//result.attr.booleanVar = 0;
		result.action = 0;
	}

	return &result;
}

control *
checkhost_1_svc(control *argp, struct svc_req *rqstp)
{
	static control  result;


	return &result;
}

form *
sendask_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

		//falta codigo para quando o grandao esta enviando a pergunta
	//if eu sou  o grandao
	if(current_user == manager)
	{
		
		CLIENT *clnt;
		form  *result_3;
		form  sendask_1_arg;

		int i;
		for(i=0;i<10;i++)
		{
			if(current_user != i)
			{
				clnt = clnt_create (list[current_user], PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					clnt_pcreateerror (list[current_user]);
					exit (1);
				}
				//send msg
				result_3 = sendask_1(&sendask_1_arg, clnt);
				if (result_3 == (form *) NULL) {
					clnt_perror (clnt, "call failed");
				}

				///ainda necessita dos tratamentos de erro
			}
		}
	}
	if(argp->attr.booleanVar)
	{
		memcpy(&ask,argp,sizeof(form));//caso eu esteja recebendo uma questao do grandao
		has_ask = 1;
	}
	else
	{
		memcpy(&result,&ask,sizeof(form));//caso eu esteja recebendo um requisicao do cliente
	}

	return &result;
}

form *
sendanswer_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

	//manda pro chefao
	if(argp->attr.booleanVar)
	{
		int index = find_by_address(argp->attr);
		memcpy(answer[index],argp,sizeof(form));
		//caso eu seja o grandao
		//chamar metodo para tratar informacoes
	}
	else
	{
		list[manager]
		//manda pro grandao
		//tratamento caso ele esteja morto
	}
	return &result;
}

infoperson *
nicetomeetyou_1_svc(infoperson *argp, struct svc_req *rqstp)
{
	static infoperson  result;

	printf("%d\n",current_user);

	return &result;
}

