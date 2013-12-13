
#pragma once

#define CLEARx0(x) memset(&(x), 0x00, sizeof(x))
#define CLEARxF(x) memset(&(x), 0xFF, sizeof(x))

#define DBGTRC 			fprintf(stderr, " *DBG..%s|%s|%d \n", __FILE__, __func__, __LINE__ );
#define DBGINT(x) 		fprintf(stderr, " *DBG..%s|%s|%d (%d)\n", __FILE__, __func__, __LINE__, x);
#define DBGSTR(x) 		fprintf(stderr, " *DBG..%s|%s|%d (%s)\n", __FILE__, __func__, __LINE__, x);

// Value Define
#define DFT_DEV_NAME			("/dev/video0")
#define DFT_SERVER_PORT			5212

#define DEV_NAME_LEN 			256
#define FILE_NAME_LEN			256
#define FILE_PATH_LEN			256

#define MAX_CLIENT_SIZE			8
#define MAX_CQUEUE_SIZE  		32