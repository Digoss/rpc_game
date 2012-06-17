
#include "trab.h"
#include "trab_clnt.c"
#include <string.h>

int has_ask = 0; //para servidor nao coordenador

form ask[3]; //perguntas enviadas pelo coordenador

form answer[10]; //respostas dadas ao coordenador

int responses[3]; //respostas das perguntas, guardada pelo coordenador

int current_user, manager;



struct user_ip{
	char name[60];
	char ip[40];
	int points;
	int keepAlive;
};

struct user_ip list[10]; //lista de ips

int
find_by_address(commomattributtes attr)
{
	int i;
	for(i=0;i<10;i++)
		if(strcmp(attr.address,list[i].ip)==0)
			return i;
	printf("Endereco nao encontrado\n");
	exit(0);
}

//fazer parse de um arquivo que contenha o endereco ip dos hosts


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

	//if eu sou  o grandao
	if(current_user == manager)
	{
		
		CLIENT *clnt;
		form  *result_3;
		form  sendask_1_arg;

		memcpy(&sendask_1_arg, argp, sizeof(form));
		responses[sendask_1_arg.next] = sendask_1_arg.answer[0];//armazena a resposta

		int i;
		for(i=0;i<10;i++)
		{
			if(current_user != i)
			{
				char *host = list[i].ip;
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					clnt_pcreateerror (host);
					//exit (1);
					continue;
				}
				//send msg
				sendask_1_arg.attr.booleanVar = 1;
				result_3 = sendask_1(&sendask_1_arg, clnt);
				if (result_3 == (form *) NULL) {
					clnt_perror (clnt, "call failed");
				}

				list[i].keepAlive = 1;
				//manter um array com os participantes que receberao a pergunta
				///ainda necessita dos tratamentos de erro
			}
		}
	}
	if(argp->attr.booleanVar)
	{
		memcpy(&ask[argp->next],argp,sizeof(form));//caso eu esteja recebendo uma questao do grandao
		has_ask = 1;
	}
	else
	{
		memcpy(&result,&ask[argp->next],sizeof(form));//caso eu esteja recebendo um requisicao do cliente
	}

	return &result;
}

form *
sendanswer_1_svc(form *argp, struct svc_req *rqstp)
{
	static form  result;

	//if(argp->attr.booleanVar)
	if(current_user == manager)
	{
		int index = find_by_address(argp->attr);
		int formsize = sizeof(form);
		memcpy(answer+index*formsize, argp, formsize);
		//caso eu seja o grandao
		//chamar metodo para tratar informacoes
	}
	//manda pro chefao
	else
	{
		memcpy(&answer[current_user],argp,sizeof(form));

		CLIENT *clnt;
		form  *result_4;
		form  sendanswer_1_arg;

		memcpy(&sendanswer_1_arg, &answer[current_user], sizeof(form));

		char *host = list[manager].ip;
		clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
		if (clnt == NULL) {
			clnt_pcreateerror (host);
			exit (1);
		}
		result_4 = sendanswer_1(&sendanswer_1_arg, clnt);
		if (result_4 == (form *) NULL) {
		 	clnt_perror (clnt, "call failed");
		}
		//manda pro grandao
		//falta tratamento caso ele esteja morto
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

