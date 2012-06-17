
#include "trab.h"
#include "trab_clnt.c"
#include <string.h>

int has_ask = 0; //para servidor nao coordenador

form ask[3]; //perguntas enviadas pelo coordenador

form answer[10]; //respostas dadas ao coordenador

int responses[3]; //respostas das perguntas, guardada pelo coordenador

int current_user;
int manager = -1;

struct user_ip{
	char name[60];
	int age;
	char ip[40];
	int points;
	int keepAlive;
};

struct user_ip list[10]; //lista de ips

int
readPlayers()
{
	int index;
	FILE *fr;
        char * line = malloc(140);

        fr = fopen ("players.txt","rt");
	index = 0;
        while(fgets(line,60,fr) != NULL)
        {
		strcpy(list[index].name, line);
		if(fgets(line,5,fr) != NULL)
		{
			list[index].age =  atoi(line);
			if(fgets(line,40,fr) != NULL)
			{
				strcpy(list[index].ip, line);
			}
		}
                index++;
        }
	return 0;
}


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
	else if(has_ask == 3)
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
	CLIENT *clnt;

	control  *result_2;
        control  checkhost_1_arg;
	
	if(argp->action == 100) //outro servidor avisa que processo corrente e o de maior prioridade vivo achado por ele
	{
		int i;
		char *host;
		strcpy(checkhost_1_arg.attr.address, list[current_user].ip);
		for(i=0;i<0;i++)
		{	
			if(i != current_user)
			{
				host = list[i].ip;
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
                               	if (clnt == NULL) {
                               		 continue;
                       		}
				checkhost_1_arg.attr.booleanVar = 1;
				result_2 = checkhost_1(&checkhost_1_arg, clnt);
		        	if (result_2 == (control *) NULL) {
					continue;
        			}
				//enviar mensagem avisando que agora o processo corrente e o coordenador
			}
		}
	}
	else if(argp->attr.booleanVar)//quando recebe aviso do novo coordenador 
	{
		manager = find_by_address(argp->attr);
	}
	else// quando recebe mensagem originada por um cliente
	{
		char *host;
		int i;
		for(i=0;i<10;i++)
		{
			if(i == current_user) //quando e o processo de maior prioridade
			{
				strcpy(checkhost_1_arg.attr.address, list[current_user].ip);
				for(i=0;i<0;i++)
				{	
					if(i != current_user)
					{
						host = list[i].ip;
						clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
	                                	if (clnt == NULL) {
       	                                		 continue;
       	                        		}
						checkhost_1_arg.attr.booleanVar = 1;
						result_2 = checkhost_1(&checkhost_1_arg, clnt);
				        	if (result_2 == (control *) NULL) {
							continue;
		        			}
						//enviar mensagem avisando que agora o processo corrente e o coordenador
					}
				}
				break;
			}
			else if(i != current_user)// procura o processo de maior prioridade e envia mensagem
			{
				host = list[i].ip;
				clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
				if (clnt == NULL) {
					continue;
		        	}
				checkhost_1_arg.action = 100; //avisa ao host destino que e o coordenador
				result_2 = checkhost_1(&checkhost_1_arg, clnt);
		        	if (result_2 == (control *) NULL) {
					continue;
		        	}
				break;
			}
		}
	}

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
		char *host;
		int i;
		for(i=0;i<10;i++)
		{
			if(current_user != i)
			{
				host = list[i].ip;
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
		has_ask++; 
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

