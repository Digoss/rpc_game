/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */


#include "trab.h"
#include <time.h>
#include <string.h>




void
progjogo_1(char *host)
{
	CLIENT *clnt;

	control  *result_1;
	control  whatdoto_1_arg;

	//control  *result_2;
	//control  checkhost_1_arg;

	form  *result_3;
	form  sendask_1_arg;

	form  *result_4;
	form  sendanswer_1_arg;

	infoperson  *result_5;
	infoperson  nicetomeetyou_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, PROGJOGO, VERJOGO, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */

	//printf("Put your name!");
	//scanf("%s",&nicetomeetyou_1_arg.name);
	//printf("Put your age!");
	//scanf("%d",&nicetomeetyou_1_arg.age);

	//result_5 = nicetomeetyou_1(&nicetomeetyou_1_arg, clnt);
	//if (result_5 == (infoperson *) NULL) {
	//	clnt_perror (clnt, "call failed");
	//}
	//if(result_5->attr.booleanVar)
	//{
		
		int i;
		while(1)
		{
			result_1 = whatdoto_1(&whatdoto_1_arg, clnt);
			if (result_1 == (control *) NULL) {
				clnt_perror (clnt, "call failed");
			}

			switch(result_1->action)
			{
				case 1://se é coordenador
					//seleciona as perguntas
					break;

				case 2://se está recebendo questões
					i=3;
					while(i--)
					{
						result_3 = sendask_1(&sendask_1_arg, clnt);
						if (result_3 == (form *) NULL) {
							clnt_perror (clnt, "call failed");
						}

						printf("%s\n",result_3->ask);

						printf("%s\n",result_3->options);

						printf("Put the number of your answer\n");
						scanf("%d",&sendanswer_1_arg.answer);
						strcpy(sendanswer_1_arg.attr.address, host);
						sendanswer_1_arg.attr.booleanVar = 0;

						result_4 = sendanswer_1(&sendanswer_1_arg, clnt);
						if (result_4 == (form *) NULL) {
							clnt_perror (clnt, "call failed");
						}

					}
					break;

				case 3://se esta recebendo resultados
					break;

				default:
					sleep(1);
			}
		}
	//}

	// result_2 = checkhost_1(&checkhost_1_arg, clnt);
	// if (result_2 == (control *) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
	// result_3 = sendask_1(&sendask_1_arg, clnt);
	// if (result_3 == (form *) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }
	// result_4 = sendanswer_1(&sendanswer_1_arg, clnt);
	// if (result_4 == (form *) NULL) {
	// 	clnt_perror (clnt, "call failed");
	// }

#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host = "localhost";

	//if (argc < 2) {
		//printf ("usage: %s server_host\n", argv[0]);
		//exit (1);
	//}
	//host = argv[1];
	progjogo_1 (host);
exit (0);
}
