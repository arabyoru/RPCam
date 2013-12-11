#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>

#include <getopt.h>

#include "RPCamMain.h"

#define APP_VER		"0.1"

RPCamMain 		theRPCamMain;

void ExitService(int nSignal)
{
	fprintf(stderr, "%d signal received.. \n", nSignal);
	switch(nSignal)
	{
	default:
	case SIGHUP:
	case SIGQUIT:
	case SIGALRM:
	case SIGPIPE:
	case SIGCHLD:
		break;
	case SIGKILL:
	case SIGTERM:
	case SIGINT:
		 fprintf(stderr, "%d Exit Signal \n", nSignal);
		exit(0);
		break;
	}
}

static void usage(FILE *fp, int argc, char **argv)
{
	fprintf(fp, "=========================================================\n");
	fprintf(fp, 
		"Usage : %s [option] \n\n"
		"Version %s \n"
		"Options : \n"
		"-h | --help 			Print This Message \n"
		"-c | --count 			Image Count [default=1] \n"
		"-p | --prefix 			Prefix of File Name \n"
		"\n"
		"e.g.) $>RPCAM -w 640 -h 480 -c 10 -p KG \n"
		,argv[0], APP_VER);
	fprintf(fp, "=========================================================\n");
}

static const char short_option[] = "c:p:h";

static const struct option long_options[] = {
	{"help", 	no_argument, 		NULL, 'h'},
	{"count", 	optional_argument, 	NULL, 'c'},
	{"prefix", 	optional_argument, 	NULL, 'p'},
	{ 0, 0, 0, 0 },
};

int main(int argc, char** argv)
{
	unsigned int nCnt=0;
	signal(SIGHUP, 	SIG_IGN);
	signal(SIGQUIT, ExitService);
	signal(SIGINT, 	ExitService);
	signal(SIGTERM, ExitService);
	signal(SIGKILL, ExitService);
	signal(SIGALRM, ExitService);
	signal(SIGPIPE, ExitService);
	signal(SIGCHLD,	SIG_IGN);

	while(true)
	{
		int nOptIdx;
		int nOptRst;
		nOptRst = getopt_long(argc, argv, short_option, long_options, &nOptIdx);

		if(nOptRst < 0) break;

		switch(nOptRst)
		{
			case 0: break;
			case 'h':
			{
				usage(stdout, argc, argv);
				break;
			}
			case 'c':
			{
				fprintf(stdout, "c : %s \n", optarg);
				nCnt = strtol(optarg, NULL, 0);
				break;
			}
			case 'p':
			{
				fprintf(stdout, "p : %s \n", optarg);
				break;
			}
			default:
				usage(stderr, argc, argv);
				exit(EXIT_FAILURE);
		}
	}

	fprintf(stdout, "Raspberry Pi Camera Start (Using V4L2) \n");

	if(theRPCamMain.InitMain(argc, argv))
	{
		if(theRPCamMain.StartMain(nCnt))
		{
			while(theRPCamMain.IsRunning()) 
			{
				sleep(1);
			}
			sleep(1);
			theRPCamMain.StopMain();			
		}
	}
	theRPCamMain.FreeMain();
	return 0;
}