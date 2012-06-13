/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "trab.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
progjogo_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		control whatdoto_1_arg;
		control checkhost_1_arg;
		form sendask_1_arg;
		form sendanswer_1_arg;
		infoperson nicetomeetyou_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case whatdoto:
		_xdr_argument = (xdrproc_t) xdr_control;
		_xdr_result = (xdrproc_t) xdr_control;
		local = (char *(*)(char *, struct svc_req *)) whatdoto_1_svc;
		break;

	case checkhost:
		_xdr_argument = (xdrproc_t) xdr_control;
		_xdr_result = (xdrproc_t) xdr_control;
		local = (char *(*)(char *, struct svc_req *)) checkhost_1_svc;
		break;

	case sendask:
		_xdr_argument = (xdrproc_t) xdr_form;
		_xdr_result = (xdrproc_t) xdr_form;
		local = (char *(*)(char *, struct svc_req *)) sendask_1_svc;
		break;

	case sendanswer:
		_xdr_argument = (xdrproc_t) xdr_form;
		_xdr_result = (xdrproc_t) xdr_form;
		local = (char *(*)(char *, struct svc_req *)) sendanswer_1_svc;
		break;

	case nicetomeetyou:
		_xdr_argument = (xdrproc_t) xdr_infoperson;
		_xdr_result = (xdrproc_t) xdr_infoperson;
		local = (char *(*)(char *, struct svc_req *)) nicetomeetyou_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	if(argc < 2)
	{
		printf("puts the current user");
		exit(0);
	}

	current_user = atoi(argv[1]);
	printf("Current user is %d",current_user);
	register SVCXPRT *transp;

	pmap_unset (PROGJOGO, VERJOGO);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, PROGJOGO, VERJOGO, progjogo_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (PROGJOGO, VERJOGO, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, PROGJOGO, VERJOGO, progjogo_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (PROGJOGO, VERJOGO, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
